set(CMAKE_TOOLCHAIN_FILE
  ../../../../3rdparty/arduino-cmake/cmake/ArduinoToolchain.cmake
  )

cmake_minimum_required(VERSION 2.8)

#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(robocom_client_test)

set(robocom_client_test_BOARD uno)
set(robocom_client_test_PORT /dev/ttyACM0)
set(robocom_client_test_SRCS
 test.cpp
 )
generate_arduino_firmware(robocom_client_test)
