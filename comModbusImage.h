/*
 *  Modified on: 28/06/2022
 *      Author: Sergey
 */

#ifndef COMMODBUSIMAGE_H_
#define COMMODBUSIMAGE_H_

#if defined(__MINGW32__) || defined(_WIN32)

#include "Image.h"
#include "iSerialPort.h"
#include "crc16.h"


namespace std {

class ComModbusImage: public Image {
public:
	ComModbusImage();
	ComModbusImage(string portname, unsigned long baudRate, int adr);
	virtual ~ComModbusImage();

	string  getSourceName() const{return port->getName();};
	bool  write (const unsigned char * src, size_t size);
	void read() override;
        void reset() override;


	std::unique_ptr<ISerialPort> port = ISerialPort::create();

	void ConnectInti(string portname, unsigned long baudRate, int adr);

	bool check_modbus_device();
	bool check_modbus_device(int adr);
	void akn();
	bool isBootloaderReady()const{return _isBootloaderReady;};
	bool isModbusDevReady()const{return _isModbusDevReady;};

private:


	void setup_loader();
	bool eraseFlash();
	bool reset_device();
        bool get_devBaseAddr();



	bool checkCRC(char * resp, int len) const;

	int modbusAdr = -1;
	bool  _isModbusDevReady = false;
	bool _isBootloaderReady = false;
};

} /* namespace std */


#endif
#endif /* COMMODBUSIMAGE_H_ */
