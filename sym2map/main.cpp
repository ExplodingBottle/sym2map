/*
 * main.cpp
 * 
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <fstream>

#include "versdesc.h"

#include "SymbolFile.h"
#include "MapFileWriter.h"
#include "ImageFile.h"

using namespace std;

static void usage() {
	cerr << "Usage: sym2map <sym-file> <map-file> [image-file]" << endl;
	cerr << "\tsym-file: The symbol file to convert" << endl;
	cerr << "\tmap-file: The output map file" << endl;
	cerr << "\timage-file: Optional image file you can use to reconstitute more data" << endl;
	cerr << "IMPORTANT: Make sure to use the good sym-file/image-file to avoid making a bad map file." << endl;
}

int main(int argc, char* argv[]) {
	cout << "sym2map debug file conversion tool version " << TEXT_FILE_VERSION << endl;
	cout << "Copyright (c) 2025 ExplodingBottle" << endl;
	cout << "Released under the MIT License. Check LICENSE in the root of the repository." << endl << endl;

	if (argc < 3 || argc > 4) {
		usage();
		return -1;
	}

	// Symbol file
	ifstream symFileStream;
	symFileStream.open(argv[1], ios::binary | ios::in);
	if (!symFileStream.is_open()) {
		cerr << "Input SYM file couldn't be opened." << endl;
		return -1;
	}
	cout << "sym2map will now convert the SYM file to the MAP file." << endl;

	SymbolFile symFile = SymbolFile(symFileStream);
	if (!symFile.ParseSymbolFile()) {
		cerr << "Parser failed with message: " << symFile.GetFailureMessage() << "." << endl;
		symFileStream.close();
		return -1;
	}

	symFileStream.close();
	cout << "Symbol file parsing is done." << endl;

	ImageFile* imageFile = nullptr;
	// Image file
	if (argc == 4) {
		cout << "Now loading informations about the image." << endl;
		ImageFile* image = new ImageFile(argv[3]);
		if (!image->LoadImageInfos()) {
			cerr << "Image file parser couldn't parse the image file.";
			delete image;
			return -1;
		}
		imageFile = image;
		cout << "Finished to load informations about the image." << endl;
	}

	// Map file

	cout << "Writing the map file." << endl;
	ofstream mapFileStream;
	mapFileStream.open(argv[2], ios::binary | ios::out);
	if (!mapFileStream.is_open()) {
		cerr << "Output MAP file couldn't be opened." << endl;
		return -1;
	}

	MapFileWriter writer = MapFileWriter(symFile, imageFile);
	if (!writer.WriteMapFile(mapFileStream)) {
		cerr << "Writer failed to write the SYM file.";
		mapFileStream.close();
		return -1;
	}
	cout << "Finished to write the map file." << endl;

	mapFileStream.close();
	return 0;

}
