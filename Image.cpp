/*
 * Image.cpp
 *
 *  Created on: 20 ����. 2021 �.
 *      Author: Sergey
 */

#include "Image.h"
#include "hexImage.h"
#include "binImage.h"
#include "comModbusImage.h"
#include <stdlib.h>
#include <vector>

namespace std {


std::unique_ptr<IImage> ImageBuilder::read(string fname){
	// Create image and read data from selected source

	auto tmp = ImageBuilder::create(fname);
	tmp.get()->read();
	return tmp;
};

std::unique_ptr<IImage> ImageBuilder::create(string fname){
	// Create image and define it source

	bool valid = fname.length()>3;

	if(valid){
		string res = fname.substr(fname.length()-3, 3);

		if(res == "bin") {
			return std::unique_ptr<IImage>(new BinImage(fname));
		};

		if(res== "hex")
		{
			return  std::unique_ptr<IImage>(new HexImage(fname));
		};

		res=fname.substr(0, 3);

		if(res=="COM" || res=="com" || res=="tty" || res=="TTY"){

			vector<string> tmp;
			string portname = "";
			int baudrate = 0;
			int modbusadr = 1;
			int size = 128000;

			char * pch = strtok ((char *) fname.data(),",");
			while (pch != NULL){
				string a = string(pch);
				// cout<<a<<endl;
				 pch = strtok (NULL, ",");
				 tmp.push_back( a);

			};

			if(tmp.size() < 5 && tmp.size() > 1){
				portname = tmp[0];
				baudrate = stoi(tmp[1]);
				if(tmp.size()>2)modbusadr = stoi(tmp[2]);
				if(tmp.size()>3)size = stoi(tmp[3]);
				auto res = std::unique_ptr<IImage>(new ComModbusImage(portname,baudrate,modbusadr));
				res->setRequstedSize(size);
				return res;
			}
		}

	}

	cout<<fname<<endl;

	return std::unique_ptr<IImage>(new nullImage());

}


int Image::write(const unsigned char * src, size_t size, string fname){

    if(size > 0){
    	valid = false;
    	this->image.clear();
		cout<< "write "<<size<<" bytes to "<<fname <<endl;
		return size;
    }
    cout<<"nothing to write"<<endl;
    return 0;
} ;

void Image::prinImageSizeInfo(){
	cout << image.size() << " byte image read"<<endl;
}

bool Image::compare(const unsigned char * sourse, size_t size)const{


	bool res = true;
	const unsigned char * dest = image.data();

	cout<<"Comparing images ... ";

	size_t var=0;
	if(size<=image.size()){
		for (var = 0; var < size; ++var) {
			if( dest[var] != sourse[var]) {res=false; break;};
		}

	}else{res=false;};

	if(res){
		cout<<"True"<<endl;
	}else{
		cout<<"False"<<endl;
		cout<<"Count="<<var<<endl;
	}

	return res;
}


} /* namespace std */
