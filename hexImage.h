/*
 * hexImage.h
 *
 *  Created on: 20 ����. 2021 �.
 *      Author: Sergey
 */

#ifndef HEXIMAGE_H_
#define HEXIMAGE_H_

#include <fstream>
#include <vector>

#include "Image.h"
#include "record.h"
#include "template.h"


namespace std {

class HexImage: public Image {
public:

	 HexImage();
	 HexImage(string fname);
	~HexImage() {};

	string  getSourceName() const{return fname;};
	virtual bool  write (const unsigned char * src, size_t size) override final;
	void  read();

private:
	void readFile(string fname);
	bool readRecord(char * buff); 			// add new line from .hex file
	void addPld(uint8_t * src, int cnt); 	//expand memory and copy new data
	void addPld(int cnt); 				 	// expand end init memory

	std::vector<record::Record> records;

	bool EOFrecord = false; 	// .hex end of file flag
	int base_adr = 0;  			// firmware base address
	int byte_count = 0; 		  	// image byte count
	string fname;


};

} /* namespace std */

#endif /* HEXIMAGE_H_ */
