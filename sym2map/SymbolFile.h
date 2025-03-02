/*
 * SymbolFile.h
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "SegmentObject.h"

using namespace std;

class SymbolFile
{
public:
	SymbolFile(ifstream& symFile);

	bool ParseSymbolFile();
	bool IsParsingDone();
	const string& GetModuleName();
	const string& GetFailureMessage();
	const vector<SegmentObject>& GetSegments();

private:
	SymbolFile();
	void SetFailureMessage(string message);
	string ReadLimitedString();
	string failureMessage;
	ifstream& symFile;
	bool fileParsed;
	vector<SegmentObject> segments;
	string moduleName;
};
