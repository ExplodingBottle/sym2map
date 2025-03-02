/*
 * SymbolObject.cpp
 *
 * SPDX-License-Identifier: MIT
 */

#include "SymbolObject.h"

SymbolObject::SymbolObject(uint32_t symbolAddress, string symbolName) : symbolAddress(symbolAddress), symbolName(symbolName) {
}

SymbolObject::SymbolObject() : symbolAddress(0), symbolName("") {

}

uint32_t SymbolObject::GetSymbolAddress() {
	return symbolAddress;
}

string SymbolObject::GetSymbolName() {
	return symbolName;
}
