/*
 * SymbolObject.h
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <string>

using namespace std;

class SymbolObject
{
public:
	SymbolObject(uint32_t symbolAddress, string symbolName);
	SymbolObject();

	uint32_t GetSymbolAddress();
	string GetSymbolName();

private:
	uint32_t symbolAddress;
	string symbolName;
};
