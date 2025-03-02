/*
 * SegmentObject.h
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <string>
#include <vector>

#include "SymbolObject.h"

using namespace std;

class SegmentObject
{
public:
	SegmentObject(uint16_t segmentNumber, string segmentName);
	SegmentObject();
	uint16_t GetSegmentNumber();
	string GetSegmentName();
	vector<SymbolObject>& GetEditableSymbolsList();
private:
	uint16_t symbolsCount;
	uint16_t segmentNumber;
	string segmentName;
	vector<SymbolObject> symbols;
};
