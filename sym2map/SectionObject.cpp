/*
 * SectionObject.cpp
 *
 * SPDX-License-Identifier: MIT
 */

#include "SectionObject.h"

SectionObject::SectionObject(string sectionName, uint32_t rvaBaseAddress, uint32_t sectionSize, uint16_t sectionNumber, bool isDataSection) :
	sectionName(sectionName),
	rvaBaseAddress(rvaBaseAddress),
	sectionSize(sectionSize),
	sectionNumber(sectionNumber),
	isDataSection(isDataSection) {

}
const string& SectionObject::GetSectionName() {
	return sectionName;
}
uint32_t SectionObject::GetRVABaseAddress() const {
	return rvaBaseAddress;
}
uint32_t SectionObject::GetSectionSize() {
	return sectionSize;
}
uint16_t SectionObject::GetSectionNumber() {
	return sectionNumber;
}
bool SectionObject::IsDataSection() {
	return isDataSection;
}
