/*
 * SegmentObject.cpp
 *
 * SPDX-License-Identifier: MIT
 */

#include "SegmentObject.h"

SegmentObject::SegmentObject(uint16_t segmentNumber, string segmentName) : segmentNumber(segmentNumber), segmentName(segmentName) {
}

SegmentObject::SegmentObject() : segmentNumber(0), segmentName("") {
}

uint16_t SegmentObject::GetSegmentNumber() {
	return segmentNumber;
}
string SegmentObject::GetSegmentName() {
	return segmentName;
}
vector<SymbolObject>& SegmentObject::GetEditableSymbolsList() {
	return symbols;
}
