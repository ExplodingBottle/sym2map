/*
 * SymbolFile.cpp
 *
 * SPDX-License-Identifier: MIT
 */

#include "SymbolFile.h"

#include <iostream>

#define READ_DATA(name, type) type name; \
								symFile.read(reinterpret_cast<char*>(&name), sizeof(type));

SymbolFile::SymbolFile(ifstream& symFile) : symFile(symFile), fileParsed(false) {

}

const string& SymbolFile::GetFailureMessage() {
	return failureMessage;
}

string SymbolFile::ReadLimitedString() {
	READ_DATA(stringSize, uint8_t);
	char* charArray = new char[stringSize + 1];
	charArray[stringSize] = '\0';
	symFile.read(charArray, stringSize);
	string modNameString(charArray);
	delete charArray;
	return modNameString;
}

void SymbolFile::SetFailureMessage(string message) {
	failureMessage = message;
}

const vector<SegmentObject>& SymbolFile::GetSegments() {
	return segments;
}

const string& SymbolFile::GetModuleName() {
	return moduleName;
}

bool SymbolFile::ParseSymbolFile() {

	symFile.seekg(0, ios::beg);
	int64_t symFileBegin = symFile.tellg();
	symFile.seekg(0, ios::end);
	int64_t symFileEnd = symFile.tellg();
	uint64_t symFileSize = (symFileEnd - symFileBegin);
	symFile.seekg(0, ios::beg);

	READ_DATA(symbolFileSize, uint32_t);
	READ_DATA(entrySegmentNumber, uint16_t);
	READ_DATA(symbolsInHeaderNumber, uint16_t);
	READ_DATA(headerTotalSize, uint16_t);
	READ_DATA(numberOfSegments, uint16_t);
	READ_DATA(firstSegmentAddress, uint16_t);
	symFile.seekg(1, ios::cur);
	const string moduleName = ReadLimitedString();

	// Determine if we use paragraphs or bytes (respectively new format and old format)
	bool newFormat = false;
	if ((symFileSize - 4) / 0x10 == symbolFileSize) {
		cout << "Symbol file is using paragraphs as a unit." << endl;
		newFormat = true;
	}
	else if (symFileSize - 4 == symbolFileSize) {
		cout << "Symbol file is using bytes as a unit." << endl;
	}
	else {
		cout << "Symbol file reports an invalid size. The file is possibly of the wrong type." << endl;
		return false;
	}


	cout << "Symbol file module name is: " << moduleName << "." << endl;

	// Read segment zero in the header

	/* Inconsistencies found between the website and the reality. In a certain symbol file there was a strange spacing */
	/* of 1 byte between the end of the name and the beginning of the address of the function. After some observations, */
	/* it seems the you need to skip one byte if the format is not in paragraphs, don't skip else */

	if (!newFormat) {
		symFile.seekg(1, ios::cur);
	}

	SegmentObject segmentZero = SegmentObject();
	for (int symbolNumber = 0; symbolNumber < symbolsInHeaderNumber; symbolNumber++) {
		READ_DATA(symbolAddress, uint16_t);
		const string symbolName = ReadLimitedString();
		SymbolObject symbol = SymbolObject(symbolAddress, symbolName);
		segmentZero.GetEditableSymbolsList().push_back(symbol);
	}

	cout << "Segment zero contained: " << symbolsInHeaderNumber << " symbols." << endl;
	vector<SegmentObject> segments;
	if (symbolsInHeaderNumber > 0) {
		segments.push_back(segmentZero);
	}

	uint16_t nextSegmentAddress = firstSegmentAddress;
	for (int i = 0; i < numberOfSegments; i++) {
		symFile.seekg(nextSegmentAddress * (newFormat ? 0x10 : 1), ios::beg);
		symFile.read(reinterpret_cast<char*>(&nextSegmentAddress), sizeof(uint16_t));
		READ_DATA(symbolsInSegment, uint16_t);
		READ_DATA(segmentSize, uint16_t);
		READ_DATA(segmentNumber, uint16_t);
		
		/* This part is really important because if you forget about it your parsing will be totally broken */
		/* This was undocumented and has been found when generating a map file with mapsym.exe -t -l and */
		/* noticing that the segment in which everything was broken was also the only 32-bit segment */
		symFile.seekg(6, ios::cur);
		READ_DATA(customSizeMark, uint8_t);
		symFile.seekg(5, ios::cur);
		
		const string segmentName = ReadLimitedString();
		SegmentObject segment = SegmentObject(segmentNumber, segmentName);
		cout << "Segment discovered: " << segmentName << "(" << (customSizeMark ? "32-bits" : "16-bits") << ") - " << symbolsInSegment << " symbols." << endl;
		for (int symbolNumber = 0; symbolNumber < symbolsInSegment; symbolNumber++) {
			uint32_t symbolAddress;
			if (customSizeMark) {
				symFile.read(reinterpret_cast<char*>(&symbolAddress), sizeof(uint32_t));
			}
			else {
				uint16_t symbAddr2;
				symFile.read(reinterpret_cast<char*>(&symbAddr2), sizeof(uint16_t));
				symbolAddress = symbAddr2;
			}
			const string symbolName = ReadLimitedString();
			SymbolObject symbol = SymbolObject(symbolAddress, symbolName);
			segment.GetEditableSymbolsList().push_back(symbol);
		}
		segments.push_back(segment);
	}
	this->moduleName = moduleName;
	this->segments = segments;
	return true;
}

bool SymbolFile::IsParsingDone() {
	return fileParsed;
}