/*
 * SectionObject.h
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <string>

using namespace std;

class SectionObject
{
public:
	SectionObject(string sectionName, uint32_t rvaBaseAddress, uint32_t sectionSize, uint16_t sectionNumber, bool isDataSection);
	const string& GetSectionName();
	uint32_t GetRVABaseAddress() const;
	uint32_t GetSectionSize();
	uint16_t GetSectionNumber();
	bool IsDataSection();
private:
	string sectionName;
	uint32_t rvaBaseAddress;
	uint32_t sectionSize;
	uint16_t sectionNumber;
	bool isDataSection;
};

