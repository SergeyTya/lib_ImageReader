/*
 * mockSerialPort.cpp
 *
 *  Created on: 3 џэт. 2022 у.
 *      Author: Sergey
 */

#include "mockSerialPort.h"

bool MockSerialPort::write(char *data, unsigned long len){
	if(len==7){
		//unsigned short int crc1 = static_cast<unsigned short int> ( static_cast<unsigned short int>(data[6] << 8) + data[5] );

		unsigned short int crc1 = static_cast<unsigned short int> ( ( (unsigned char) data[6] << 8) + (unsigned char) data[5] );
		unsigned short int crc2 = crc16::usMBCRC16(data, 5);

		if( crc1 == crc2) {
			read_sym = 'X'; return true;
		}
	}

	if(read_sym == 'P'){
		auto itr = image.end();
		image.insert(itr, data, data+256);
		read_sym = 'N';
	};

	return true;
}

bool MockSerialPort::write(char data){
	read_sym = data;
	if(read_sym == 'A'){
		it = 0;
		};
	return true;
}

std::string MockSerialPort::readAll(){


	if(read_sym == 'I'){
		return "MOCK_LOADER";
	}
	if(read_sym == 'E'){
		it=0;
		image.clear();
		return "EOK";
	}
	if(read_sym == 'X'){
	return std::string(devid, 60);
	}

	return "";
};

unsigned long MockSerialPort::read(char *data, unsigned long maxNumBytesRead){

	if(read_sym == 'V')if(image.size()>it){

		memcpy(data,image.data()+it,maxNumBytesRead);
		it+=maxNumBytesRead;
	};
	return 0;
};
