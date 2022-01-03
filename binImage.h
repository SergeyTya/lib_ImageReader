/*
 * binImage.h
 *
 *  Created on: 20 ����. 2021 �.
 *      Author: Sergey
 */

#ifndef BINIMAGE_H_
#define BINIMAGE_H_

#include "Image.h"
#include <fstream>

namespace std {

class BinImage: public Image{
public:
	BinImage(string fname);
	virtual ~BinImage(){};

	string   getSourceName() const{return fname;};

	bool  write (const unsigned char * src, size_t size);
	void  read  ();

private:
	void readFile(string fname);
	string fname;
};


} /* namespace std */

#endif /* BINIMAGE_H_ */
