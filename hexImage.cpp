/*
 * hexImage.cpp
 *
 *  Created on: 20 ����. 2021 �.
 *      Author: Sergey
 */

#include "hexImage.h"
#include <vector>

namespace std {
HexImage::HexImage(string fname):fname(fname) {

}

void HexImage::read(){

	Image::read();
	ifstream inf(fname);

		cout << "Open " << fname << endl;

		if (inf.is_open()) {

			char *buff = new char[255];
			bool res = true;
			image.clear();

			while (inf && res) {
				inf.getline(buff, 255);
				res = readRecord(buff);
			}



			inf.close();
			delete[] buff;
			records.clear();
			this->prinImageSizeInfo();

		} else {
			cout << "Can not open file" << endl;
			valid = false;
		};
}


bool HexImage::readRecord(char*buff){

	int index = records.size();

	if(EOFrecord)return false;

	record::Record tmp = record::Record(buff);


	if(tmp.getValidState() == false){
		cout<<"Not Valid Record #"<<index<<endl;
		valid = false;
		return false;
	}

	records.push_back(tmp);

	switch (tmp.getRecordType()) {

	default:
		break;

	case record::Record::RecordType::DATA_RECORD:
		if((byte_count&0xFFFF)<tmp.getPldStartAdr()){
			//add empty bytes if record address greater than current image size
			addPld(tmp.getPldStartAdr() - (byte_count&0xFFFF));
		};

		addPld(tmp.getPld(), tmp.getPldSize());

		break;

	case record::Record::RecordType::END_OF_FILE:
		EOFrecord = true;
		return false;
		break;

	case record::Record::RecordType::ADV_SEGMENT_ADR:
		int adr = (tmp.getPld()[1] + (tmp.getPld()[0] << 8))<<16;

		if (index > 0) {
			// add empty bytes if base address is changing
			addPld(adr - (base_adr + byte_count));
			//cout << "Flash base address " << int_to_hex(adr) << endl;
		} else {
			base_adr = adr; // setup image base address
			cout << "Flash base address " << my_tmp::int_to_hex(adr) << endl;
		}

		break;
	}


	return true;

};

void HexImage::addPld(int cnt) {
	if (cnt <= 0) return;
	auto it = image.end();

	image.insert(it, cnt, 0);
	byte_count += cnt; //byte counter

	cout << "Warning: " << cnt << " empty byte added " << endl;
}

void HexImage::addPld(uint8_t * src, int cnt){
	 auto it = image.end();
	 image.insert(it, src, src+cnt);
	 byte_count += cnt; //byte counter
};


string createHexDataRecord(int16_t * pntr, int count, int adr) {

	std::stringstream stream;
	uint8_t crc = 0;
	uint8_t hdr[4] = { 0x10, 0, 0, 0 }; // record header
	*((uint16_t*) &hdr[1]) = _byteswap_ushort(adr);
	crc = hdr[0] + hdr[1] + hdr[2] + hdr[3];
	stream << ":";
	stream << setfill('0') << std::setw(sizeof(uint16_t) * 2) << hex
			<< _byteswap_ushort(((uint16_t*) hdr)[0]);
	stream << setfill('0') << std::setw(sizeof(uint16_t) * 2) << hex
			<< _byteswap_ushort(((uint16_t*) hdr)[1]);

	for (int el = 0; el < int(count / sizeof(uint16_t)); ++el) { //

		uint16_t dtr = _byteswap_ushort(pntr[el]);

		crc += ((uint8_t*) &dtr)[0] + ((uint8_t*) &dtr)[1];
		stream << setfill('0') << std::setw(sizeof(uint16_t) * 2) << hex << dtr;
	}

	stream<<std::setw(2)<<uint16_t(0xFF&(0-crc))<<endl;

	return stream.str();
}


bool HexImage::write(const unsigned char * src, size_t size) {

	if(size==0) return false;

	EOFrecord = false; 	// reset flags
	base_adr = 0;
	byte_count = 0;

	ofstream ofile(fname);

	char crc = 0;
	int record_pld_byte_count = 16; // 16 bytes pld in record
 	int pld_cnt=0;                  // pay load data count
 	uint16_t line_adr = 0;          // hex file line address
 	uint16_t page_cnt = 0;          // hex file page count


	for (int el = 0; el < size; el++){	//start record

		if(pld_cnt==record_pld_byte_count || pld_cnt==0){
			uint8_t hdr[4] = {0,0,0,0}; 			// record header
			pld_cnt=0;

			if (line_adr == 0) { // new base address page
				crc = 0 - (0x2 + 0x4 + 0x8 + page_cnt);
				ofile << ":0200000408";
				ofile << setfill('0') << std::setw(sizeof(uint8_t) * 2) << hex<< page_cnt << (uint16_t(crc) & 0xFF) << endl;
				page_cnt++;
			}

			if(size-el<record_pld_byte_count) record_pld_byte_count = size-el; // data count less than requested

			hdr[0] = record_pld_byte_count; 		// line byte count
			hdr[1] = ((uint8_t *) &line_adr)[1]; 	//
			hdr[2] = ((uint8_t *) &line_adr)[0]; 	// line data address

			crc = hdr[0]+hdr[1]+hdr[2]+hdr[3]; 		// reset crc

			ofile<<":"; 							// start symbol
			ofile<<setfill('0')<<std::setw(sizeof(uint32_t)*2)<<hex<<_byteswap_ulong(((uint32_t *)hdr)[0]); // write byte count, address and line type

			line_adr += record_pld_byte_count;
		}

		ofile<<setfill('0')<<std::setw(sizeof(uint8_t)*2)<<hex<<(uint16_t(src[el])&0xFF); //write pld data
		crc+=src[el];
		pld_cnt++;

		if(pld_cnt==record_pld_byte_count){ // end record
			ofile<<std::setw(2)<<uint16_t(0xFF&(0-crc)); //write crc
			ofile<<endl;
		}

	}

	ofile<<":00000001FF\n"<<endl;
	ofile.close();
	ofile.flush();

	read();
	bool cmpr = compare(src, size);
	if(cmpr){
		cout<<"file write successful"<<endl;
	}else{
		cout<<"file write with ERROR"<<endl;
	}
	return cmpr;
};

} /* namespace std */
