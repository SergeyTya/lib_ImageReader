/*
 * record.cpp
 *
 *  Created on: 20 ����. 2021 �.
 *      Author: Sergey
 */

#include "record.h"

namespace record {

record::Record::Record(char * buff) {
	line =std::string(buff);

//	cout<<line<<endl;

	std::string tmp = std::string(&buff[1]);
	int size = tmp.length()/2;
	int crc_pos = size - 1;
	uint8_t crc = 0;

	if (line[0] == ':') {
		valid = true;
		recordType = Record::RecordType::NO_REC;
	}else{
		valid = false;
		recordType = Record::RecordType::LINE_START_ERR;
		std::cout<<"Record start error:" <<line<<std::endl;
		return;
	};

	for(int i=0; i<size; i++){
		dataLine.push_back(uint8_t(stoi(tmp.substr(i * 2, 2), 0, 16)));
		if(i!=crc_pos) crc += dataLine[i];
	};
	crc = (0xff & (0 - crc));

	if (dataLine[crc_pos] == crc) {
		valid = true;
		dataByteCount = dataLine[0];
		pldStartAdr = (dataLine[1] << 8) + dataLine[2];
		recordType = RecordType(dataLine[3]);
	} else {
		valid = false;
		recordType = Record::RecordType::CRC_ERR;
		std::cout<<"Record CRC error:" <<line<<std::endl;
		return;
	}

	return ;
};


void Record::logPld() const {
//	for (int i = 0; i < dataByteCount; i++) {
//		cout << int_to_hex(uint16_t(*(getPld() + i)));
//		cout << endl;
//	}
}


} /* namespace std */
