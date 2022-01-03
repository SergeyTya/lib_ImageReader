/*
 * Image.h
 *
 *  Created on: 20 ����. 2021 �.
 *      Author: Sergey
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <memory>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdint.h>
#include <cstring>
#include <fstream>
#include <ostream>


namespace std {

class IImage;
class Image;
class nullImage;

struct ImageBuilder {

	static std::unique_ptr<IImage> create(string fname);
	static std::unique_ptr<IImage> create(string portname, unsigned long baudRate, int adr);


	static std::unique_ptr<IImage> read(string fname);
	static std::unique_ptr<IImage> read(string portname, unsigned long baudRate, int adr, size_t size);

};

class IImage {

public:

	virtual void	   setRequstedSize(size_t size) = 0;

	virtual string     getSourceName()          const=0;
	virtual const unsigned char *  getImage()   const=0;
	virtual size_t     getImageSize()           const=0;
	virtual bool       getImageValidState()     const=0;

	virtual bool       compare(const unsigned char * src, size_t size)  const=0;
	virtual bool       write  (const unsigned char * src, size_t size)       =0;
	virtual void       read ()                       =0;

	virtual ~IImage() = default;


};

class Image: public IImage {
public:

	~Image(){};
	Image(){};

	bool compare(const unsigned char * pntr, size_t size) const;
	const unsigned char * getImage() const{return  image.data(); }
	size_t   getImageSize()     const{ return image.size(); };
	void     setRequstedSize(size_t size){_requstedSize = size;};
	bool 	 getImageValidState() const{return valid;};

protected:

	void prinImageSizeInfo();
	bool     getValidState()    const{ return valid; }
	int      write (const unsigned char * src, size_t size, string fname);  // check image before write

	unsigned int getImageBaseAdr(){return imageBaseAdr;};
	size_t getRequstedSize(){return _requstedSize;};
	void setImageBaseAdr(unsigned int  adr){imageBaseAdr = adr;};
	void read(){image.clear(); valid = false;};


	std::vector<uint8_t> image;
	bool valid = false;  // image valid flag

private:
	unsigned int imageBaseAdr = 0x8000000;
	size_t _requstedSize = 128000;




};

class nullImage: public Image {
public:
	nullImage(){cout<<"null -"<<endl;};
	~nullImage(){};
	string   getSourceName() const{return nullptr;};
	bool write (const unsigned char * src, size_t size){return false;};
	void read(){};
};



} /* namespace std */

#endif /* IMAGE_H_ */
