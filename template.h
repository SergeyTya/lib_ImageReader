/*
 * template.h
 *
 *  Created on: 20 но€б. 2021 г.
 *      Author: Sergey
 */

#ifndef TEMPLATE_H_
#define TEMPLATE_H_

#include <string>
#include <iomanip>

namespace my_tmp{

template<typename T>
std::string int_to_hex(T i) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(sizeof(T)) << std::hex<< i;
	return stream.str();
}

}



#endif /* TEMPLATE_H_ */
