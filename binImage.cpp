/*
 * binImage.cpp
 *
 *  Created on: 20 ����. 2021 �.
 *      Author: Sergey
 */

#include "binImage.h"

namespace std {

BinImage::BinImage(string fname): fname(fname) {

	//readFile(fname);

};

void BinImage::read() {
	streampos size;
	char *memblock;

	Image::read();

	fstream bin(fname, ios::in | ios::binary | ios::ate);

	cout << "Open " << fname << endl;

	if (!bin) {
		cout << "Can not open file" << endl;
		valid = false;
		return;
	}

	if (bin.is_open()) {
		size = bin.tellg();

		memblock = new char[size];
		bin.seekg(0, ios::beg);
		bin.read(memblock, size);
		bin.close();

		auto it = image.end();
		image.insert(it, memblock, memblock + size);

		delete[] memblock;

		this->prinImageSizeInfo();

	} else
		cout << "Unable to open file";
}

bool BinImage::write(const unsigned char * src, size_t size) {

	Image::write(src, size, fname);
	if (size == 0) return false;

	ofstream ofile(fname, ios::binary | ios::out);
	ofile.clear();
	ofile.write((char*) src, size);
	ofile.close();
	ofile.flush();
//	_writeImageData((char*) image->getImage(), image->getImageSize());
//	return true;

	read();
	bool cmpr = compare(src, size);
	if (cmpr) {
		cout << "file write successful" << endl;
	} else {
		cout << "file write with ERROR" << endl;
	}
	return cmpr;

	}
} /* namespace std */
