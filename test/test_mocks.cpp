/*
 * test_COMimage.cpp
 *
 *  Created on: 29 дек. 2021 г.
 *      Author: Sergey Tyagushev
 */



#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "comModbusImage.h"
#include "win_port.h"
#include "Image.h"
#include "crc16.h"
#include "iSerialPort.h"

using testing::AtLeast;
using testing::AtMost;
using testing::Return;
using testing::_;

struct MockSerialPort : std::ISerialPort {

    MOCK_METHOD(
        bool, openPort,(std::string portname, unsigned long baudRate), (override)
    );

	MOCK_METHOD(
		bool, isOpen,(),(override)
    );

	MOCK_METHOD(
		void, close,(),()
    );

	MOCK_METHOD(
			unsigned long, bytesToRead,(),()
	    );

	MOCK_METHOD(
			bool, waitForReadyRead,(int timeout),(override)
	    );

	MOCK_METHOD(
			bool, waitForReadyRead,(int timeout , unsigned long size),(override)
	    );

	MOCK_METHOD(
			std::string, readAll,(),(override)
	    );

	MOCK_METHOD(
			std::string, readAll,(int timeout),(override)
	    );

	MOCK_METHOD(
			bool, write,(char *data, unsigned long len),(override)
	    );

	MOCK_METHOD(
			bool, write,(char data),(override)
		    );

	MOCK_METHOD(
			std::string, getName,(),( const override)
		    );

	MOCK_METHOD(
			unsigned long, read,(char *data, unsigned long maxNumBytesRead),( override)
		    );



};

TEST(COMdev, akn) {

	// Arrange
	MockSerialPort * mockSerialPort = new MockSerialPort();

   // Prepare mocks
   EXPECT_CALL(mockSerialPort, write('A'))
               .Times(AtLeast(1))
    		 .WillOnce(Return(true));

   EXPECT_CALL(mockSerialPort, waitForReadyRead(5))
               .Times(AtLeast(2))
			 .WillOnce(Return(true));

   EXPECT_CALL(mockSerialPort, write(_,4))
               .Times(AtLeast(1))
			  .WillOnce(Return(true));

   EXPECT_CALL(mockSerialPort, waitForReadyRead(_))
               .Times(AtLeast(2))
			  .WillOnce(Return(true));

   EXPECT_CALL(mockSerialPort, readAll())
               .Times(AtLeast(1))
			  .WillOnce(Return("xxx"));

   // Act
   std::ComModbusImage mockdev = std::ComModbusImage();
   mockdev.port = std::move(mockSerialPort);
   mockdev.akn();

   // Assert

}

TEST(COMdev, write) {

	// Arrange
	MockSerialPort  mockSerialPort;

    std::ComModbusImage mockdev = std::ComModbusImage();
    mockdev.port = &mockSerialPort;

   // Prepare mocks
   EXPECT_CALL(mockSerialPort, write('A'))
               .Times(AtLeast(1))
    		 .WillOnce(Return(true));

   EXPECT_CALL(mockSerialPort, waitForReadyRead(5))
               .Times(AtLeast(2))
			 .WillOnce(Return(true));

   EXPECT_CALL(mockSerialPort, write(_,4))
               .Times(AtLeast(1))
			  .WillOnce(Return(true));

   EXPECT_CALL(mockSerialPort, waitForReadyRead(_))
               .Times(AtLeast(2))
			  .WillOnce(Return(true));

   EXPECT_CALL(mockSerialPort, readAll())
               .Times(AtLeast(1))
			  .WillOnce(Return("xxx"));

   // Act
   mockdev.akn();

   // Assert

}
