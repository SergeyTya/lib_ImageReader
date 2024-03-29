cmake_minimum_required(VERSION 3.19)
project(imagereader_lib)

if(TEST_IMAGEREADER)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}") # puts libriries and bin at the same dir
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
endif()

add_subdirectory(lib_crc)
add_subdirectory(lib_SerialPort)

option(TEST_IMAGEREADER "build test bin" OFF)
option(DEBUG "debug configuration" OFF)

if(DEBUG)
       set(CMAKE_BUILD_TYPE Debug)
endif()

if(TEST_IMAGEREADER)

 # googletest
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
    "googletest"
)

add_executable(
    test_imagereader
    test/test.cpp
    test/MockSerialPort.cpp
    Image.cpp
    binImage.cpp
    hexImage.cpp
    record.cpp
    comModbusImage.cpp
    Image.h
    binImage.h
    hexImage.h
    record.h
    comModbusImage.h
)

# Add src to the include directories for the test target
target_include_directories(
     test_imagereader PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Link test target against gtest libraries
target_link_libraries(
        test_imagereader PRIVATE
        gtest gtest_main
        lib_crc
        lib_SerialPort
)

target_compile_options(
        test_imagereader PRIVATE
        -Wall
        -Wextra
        -pedantic
        -std=c++17
        -lserialport
        #-Werror
)

else()

add_library( imagereader STATIC
hexImage.h
hexImage.cpp
record.h
record.cpp
comModbusImage.h
comModbusImage.cpp
Image.h
Image.cpp   
binImage.h
binImage.cpp 
)

#USE interface to unclude lib path at project target
target_include_directories(imagereader
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
          

target_include_directories(
     imagereader PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)


target_link_libraries(
    imagereader PRIVATE
        lib_SerialPort
    lib_crc
)

install(TARGETS imagereader DESTINATION .)

endif()	