
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <gtest/gtest.h>
#include "record.h"
#include "comModbusImage.h"
#include "win_port.h"
#include "Image.h"
#include "mockSerialPort.h"

TEST(hexImage, record_check) {
    // Arrange
	char buf[] = ":020000040800F2" ;
	record::Record tmp = record::Record(buf);

	// Act (empty for this test)

    // Assert
   ASSERT_TRUE(tmp.getValidState());
}


TEST(comModbusImage, mock_write) {
    // Arrange
	auto binIm = std::ImageBuilder::read("test.bin");
	std::ComModbusImage mockdev = std::ComModbusImage();
	mockdev.port = std::move(std::unique_ptr<std::ISerialPort> (new MockSerialPort()));

	// Act (empty for this test)
	mockdev.ConnectInti("xx", 9600, 1);
	bool res = mockdev.write(binIm->getImage(), binIm->getImageSize());

    // Assert
	ASSERT_TRUE(mockdev.isModbusDevReady());
	ASSERT_TRUE(mockdev.isBootloaderReady());
	ASSERT_TRUE(res);
}



int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
