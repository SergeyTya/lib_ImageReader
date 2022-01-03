/*
 * mockSerialPort.h
 *
 *  Created on: 3 џэт. 2022 у.
 *      Author: Sergey
 */

#ifndef TEST_MOCKSERIALPORT_H_
#define TEST_MOCKSERIALPORT_H_

#include "crc16.h"
#include "iSerialPort.h"
#include "vector"
#include "string"
#include "Image.h"

struct MockSerialPort: public std::ISerialPort
{

		 MockSerialPort(){

			memset(devid,'x',60);
			crc16::usMBCRC16(devid,58);

			auto binIm = std::ImageBuilder::read("test.bin");
			image.clear();
			image.assign(binIm->getImage(),binIm->getImage()+binIm->getImageSize());

		 };

		 bool openPort(std::string portname, unsigned long baudRate){return true;};

		 unsigned long bytesToRead(void){return 0;};

		 bool isOpen(){return true;};
		 bool waitForReadyRead(int timeout){return true;};
		 bool waitForReadyRead(int timeout, unsigned long size){return true;};

		 bool write(char *data, unsigned long len);
		 bool write(char data);
		 unsigned long read(char *data, unsigned long maxNumBytesRead);
		 std::string readAll();
		 std::string readAll(int timeout){return readAll();};

		 std::string getName() const{return " ";};
		 void close(){};

		 ~MockSerialPort(){};

		 std::vector <char> image;
		 char read_sym = 'A';

		 int it =0;

		char devid[60];


};


#endif /* TEST_MOCKSERIALPORT_H_ */
