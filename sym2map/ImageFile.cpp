/*
 * ImageFile.cpp
 *
 * SPDX-License-Identifier: MIT
 */

#include "ImageFile.h"

#include <Windows.h>
#include <iostream>

ImageFile::ImageFile(string imagePath) : imagePath(imagePath) {
}

bool ImageFile::LoadImageInfos() {
	const char* fileName = imagePath.c_str();

	HANDLE imageHandle = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (imageHandle == INVALID_HANDLE_VALUE) {
		cout << "Couldn't open image file with error: 0x" << hex << GetLastError();
		cout << "." << endl;
		return false;
	}

	HANDLE imageFileMapping = CreateFileMappingA(imageHandle, NULL, PAGE_READONLY, 0, 0, NULL);
	if (imageFileMapping == NULL) {
		cout << "Couldn't create image file mapping with error: 0x" << hex << GetLastError();
		cout << "." << endl;
		CloseHandle(imageHandle);
		return false;
	}

	LPVOID imageFileBaseAddress = MapViewOfFile(imageFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (imageFileBaseAddress == NULL) {
		cout << "Couldn't map the view of the file with error: 0x" << hex << GetLastError();
		cout << "." << endl;
		CloseHandle(imageFileMapping);
		CloseHandle(imageHandle);
		return false;
	}

	boolean succeed = false;
	PIMAGE_DOS_HEADER fileDosHeader = (PIMAGE_DOS_HEADER)imageFileBaseAddress;
	if (fileDosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
		PIMAGE_NT_HEADERS32 fileNTHeader = (PIMAGE_NT_HEADERS32)((u_char*)fileDosHeader + fileDosHeader->e_lfanew);
		if (fileNTHeader->Signature == IMAGE_NT_SIGNATURE) {
			DWORD imageBaseAddr = fileNTHeader->OptionalHeader.ImageBase;

			DWORD entryPointAddr = fileNTHeader->OptionalHeader.AddressOfEntryPoint;
			WORD numberOfSections = fileNTHeader->FileHeader.NumberOfSections;

			PIMAGE_SECTION_HEADER imageSectionHeader = IMAGE_FIRST_SECTION(fileNTHeader);
			for (uint16_t i = 0; i < numberOfSections; i++) {
				IMAGE_SECTION_HEADER curHeader = imageSectionHeader[i];
				string name(reinterpret_cast<char*>(curHeader.Name), 0, 8);
				bool isDataSection = (imageSectionHeader[i].Characteristics & IMAGE_SCN_CNT_CODE) == 0;
				SectionObject section = SectionObject(name, curHeader.VirtualAddress + imageBaseAddr, curHeader.Misc.VirtualSize, i + 1, isDataSection);
				if (curHeader.VirtualAddress <= entryPointAddr && entryPointAddr < curHeader.VirtualAddress + curHeader.Misc.VirtualSize) {
					entryPointSegmentNumber = i + 1;
					entryPointOffset = entryPointAddr - curHeader.VirtualAddress;
					foundEntryPoint = true;
				}
				sections.push_back(section);
			}
			loadAddress = imageBaseAddr;
			succeed = true;
		}
	}

	UnmapViewOfFile(imageFileBaseAddress);
	CloseHandle(imageFileMapping);
	CloseHandle(imageHandle);

	return succeed;
}

const vector<SectionObject>& ImageFile::GetSections() {
	return sections;
}

uint32_t ImageFile::GetLoadAddress() {
	return loadAddress;
}

uint32_t ImageFile::GetEntryPointOffset() {
	return entryPointOffset;
}

uint16_t ImageFile::GetEntryPointSegmentNumber() {
	return entryPointSegmentNumber;
}

bool ImageFile::IsEntryPointPresent() {
	return foundEntryPoint;
}