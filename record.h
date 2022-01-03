/*
 * record.h
 *
 *  Created on: 20 ����. 2021 �.
 *      Author: Sergey
 */

#ifndef RECORD_H_
#define RECORD_H_

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdint.h>
#include <cstring>

namespace record {

class Record {

public:

	enum class RecordType {
			CRC_ERR=-3,
	        LINE_START_ERR,
			NO_REC,
			DATA_RECORD,
	        END_OF_FILE ,
	        SEGMENT_ADR ,
			RUN_ADR80x86 ,
			ADV_SEGMENT_ADR ,
			RUN_ADR
	    };
	Record(){};
	Record(char * buff);

	RecordType getRecordType()     const{return recordType; };
	int        getPldStartAdr()    const{return pldStartAdr;};
	int 	   getPldSize()		   const{return dataByteCount;};
	uint8_t *  getPld() 		   const{return (uint8_t*) dataLine.data() + 4;};
	bool       getValidState()     const{return valid;};
	std::string     getLine()           const{return line;};


	void logPld() const;

private:

	std::string line;  				// records string line from
	std::vector <uint8_t> dataLine;	// records data line
	bool valid = false;			// records valid flag
	int dataByteCount = 0;		// records pay load byte count
	int pldStartAdr = 0;        // records pay load flash address

	RecordType recordType = Record::RecordType::NO_REC;

};

} /* namespace std */

#endif /* RECORD_H_ */
