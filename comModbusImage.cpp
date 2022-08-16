/*
 *  Modified on: 28/06/2022
 *      Author: Sergey
 */

#include "comModbusImage.h"

namespace std {


ComModbusImage::ComModbusImage(){
}

ComModbusImage::ComModbusImage(string portname, unsigned long baudRate, int adr){
	ConnectInti(portname, baudRate, adr);
};

ComModbusImage::~ComModbusImage() {
	if(port->isOpen())port->close();
}

void ComModbusImage::ConnectInti(string portname, unsigned long baudRate, int adr){


	this->modbusAdr = adr;
	cout<<"Searching device "<<portname<<" "<<baudRate<<" "<<adr<<endl;
	port->openPort(portname,baudRate);

  if(port->isOpen()){
		check_modbus_device();
		setup_loader();
		akn();
	};
}

bool ComModbusImage::check_modbus_device() {

	try {

		if(!port->isOpen()) throw std::runtime_error("Port is close");
		if(modbusAdr<1 || modbusAdr > 255) throw std::runtime_error("Wrong modbus address");

		char req[] = { static_cast<char>(modbusAdr), 0x2b, 0xe, 0x1, 0x1, 0, 0 };

		cout << "Searching ModBus device at address " << modbusAdr << " ... ";

		crc16::usMBCRC16(req, 5);

		port->readAll();
		port->write(req, 5 + 2);

		string str = port->readAll(15);
		if(str.length()<4 || !checkCRC((char *)str.data(), str.length()))
				throw std::runtime_error("CRC mismatch");

		cout << "ok" << endl;
		cout << "Device: " << str.substr(10, 8) << str.substr(32, 10) << str.substr(44, 9) << endl;

		_isModbusDevReady = true;
		return true;

	} catch (std::runtime_error &e) {
		std::cerr << "Error: " << e.what() << '\n';
		return false;
	}

}

bool ComModbusImage::check_modbus_device(int adr){
	modbusAdr = adr;
	return check_modbus_device();
}

void ComModbusImage::setup_loader() {

	try {

		char req2[] = { static_cast<char>(modbusAdr), 0x06, 0x0, 0x0, 0x77,
				0x77, 0, 0 };
                if(_isModbusDevReady){
                  cout << "Got to loader ... ";
                  crc16::usMBCRC16(req2, 6);

                  port->write(req2, 6 + 2);
                  cout << "ok \n ";
                }

		cout << "Setup loader ... ";

                string str;
                for (int i = 0; i < 2; ++i) {
                  port->write('I');
                  str = port->readAll(50);
                }

		cout << str << endl;

                get_devBaseAddr();

		_isBootloaderReady = true;

	} catch (std::runtime_error &e) {
		std::cerr << "Error: " << e.what() << '\n';
		_isBootloaderReady = false;
	}

}

void ComModbusImage::read(){

	char buf[512];
	Image::read();
	size_t size = getRequstedSize();
	try{
		if(!_isBootloaderReady)throw std::runtime_error("device is not ready");

		this->akn();

		cout << "Reading " <<this->getRequstedSize()<<" byte image from device ";

		int tmp_cnt = size / 256;

		if (size % 256 != 0)
			tmp_cnt += 1; // if reference image size not multiple 256
                int temp_pr = 0;
		for (int i = 0; i < tmp_cnt; i++) {


			port->write('V');
			port->waitForReadyRead(5, 256);
			port->read(buf, 256);

			int page_size = 256;
			auto it = image.end();
			if (i == int(size / 256)) page_size =int(256 - (tmp_cnt * 256 - size));

			this->image.insert(it, buf, buf + page_size);

			int progres = 100 * (i + 1) / int(size / 256);
                        if( (temp_pr/10)!=(progres/10)){
                          cout << ". ";
                          temp_pr=progres;
                        }

		}

		this->prinImageSizeInfo();
		return;

	}catch(std::runtime_error &e){
		std::cerr << "Error: " << e.what() << '\n';
	}

}

bool ComModbusImage::write(const unsigned char * src, size_t size) {
	try {

		if(!_isBootloaderReady)throw std::runtime_error("device is not ready");
		char buf[512];

		Image::write(src,size, port->getName());
		if (size == 0) throw std::runtime_error("Can not write empty image");
		this->akn();

		this->eraseFlash();

		this->akn();

		int tmp_cnt = int(size) / 256;
		if (size % 256 != 0) tmp_cnt += 1; // if reference image size not multiple 256

		cout << "Writing Image to device ";

                int temp_pr = 0;
		for (int i = 0; i < tmp_cnt; i++) { // writing data

			port->write('P');

			//port->readAll(3);

			if (i != int(size) / 256) { // if reference image size not multiple 256

				port->write( (char *) (src + i * 256), 256);

			} else {

				int tmp_sz_init =int(size) - (tmp_cnt-1) * 256; // mem size have to be copied
                                memset(buf, 0xFF, 256);
                                memcpy(buf, (char *) (src + i * 256), tmp_sz_init);
                                port->write(buf, 256);
                        }

                        port->readAll(3);
                        int progres = 100 * (i + 1) / int(size / 256);
                        if ((temp_pr / 10) != (progres / 10)) {
                          cout << ". ";
                          temp_pr = progres;
                        }
                }
		cout<<"Done"<<endl;
		setRequstedSize(size);
		this->read();
		if(this->compare(src, size)){
			cout<<"done"<<endl;
			return true;
		}else{
			cout<<"fail"<<endl;
		};

	} catch (std::runtime_error &e) {
		std::cerr << "Error: " << e.what() << '\n';
		return false;
	}

	return false;
}


bool ComModbusImage::reset_device(){

	char buf[512];
		string str = "";

		cout<<"Restart MCU ... ";

		buf[0]='R';
		buf[1]='R';
		if (port->write(buf, 1) == false)
				return -1;

		while (port->waitForReadyRead(10)) {
			str += port->readAll();
		};

		cout<<"ok"<<endl;

		return 0;
}

void ComModbusImage::akn(){

	char buf[4] =
		{		0,
				0,
				static_cast<char>(getImageBaseAdr() >> 16),
				static_cast<char>(getImageBaseAdr() >> 24)
		};

	port->write('A');
	port->waitForReadyRead(5);
	port->write(buf, 4);
	port->waitForReadyRead(1);
	port->readAll();

}


bool ComModbusImage::eraseFlash(){

	this->akn();
	cout<<"Erase MCU flash ... ";
	port->waitForReadyRead(3);
	port->readAll();
	port->write('E');
	string str = port->readAll(10);
	if(str!="EOK") {
		throw std::runtime_error(" Flash erase error");
	}

	cout<<"ok"<<endl;

	return true;
}


bool ComModbusImage::checkCRC(char * resp, int len) const{

	char tmp[len];
	bool res =true;
	memcpy(tmp, resp, len);
	crc16::usMBCRC16(tmp, len-2);

	for (int var = 0; var < len; ++var) {
		if(resp[var]!=tmp[var]) res=false;
	}

	return res;
}


bool ComModbusImage::get_devBaseAddr() {
  this->akn();
  cout<<"Get MCU flash start addr... ";
  port->waitForReadyRead(3);
  port->readAll();
  port->write('B');
  string str;
  try{
    str = port->readAll(10);
    if(str.length()!=2) throw std::runtime_error("");
    if(str[0] != 'B') throw std::runtime_error("");
    this->setImageBaseAdr(0);
  }catch (std::runtime_error &e) {
    str = "0x8000000 default";
    this->setImageBaseAdr(0x8000000);
  }
  cout<<str<<"\n";

  return false;

}
void ComModbusImage::reset() {
  Image::reset();
  this->akn();
  reset_device();
}

} /* namespace std */
