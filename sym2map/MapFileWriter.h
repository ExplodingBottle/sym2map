/*
 * MapFileWriter.h
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <fstream>
#include <iomanip>

#include "SymbolFile.h"
#include "ImageFile.h"

class MapFileWriter
{
public:
	MapFileWriter(SymbolFile& symbolInformations, ImageFile* imageFile);
	bool WriteMapFile(ofstream& output);
private:
	SymbolFile& symbolInformations;
	ImageFile* imageFile;
};

