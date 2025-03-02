/*
 * ImageFile.h
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <string>
#include <vector>
#include "SectionObject.h"

using namespace std;

class ImageFile
{
public:
	ImageFile(string imagePath);
	bool LoadImageInfos();
	const vector<SectionObject>& GetSections();
	uint32_t GetLoadAddress();
	uint32_t GetEntryPointOffset();
	uint16_t GetEntryPointSegmentNumber();
	bool IsEntryPointPresent();

private:
	string imagePath;
	uint32_t loadAddress;

	bool foundEntryPoint;
	uint32_t entryPointOffset;
	uint16_t entryPointSegmentNumber;
	vector<SectionObject> sections;
};

