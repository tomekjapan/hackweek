set(CMAKE_TOOLCHAIN_FILE
  ../../../../../arduino-cmake/cmake/ArduinoToolchain.cmake
  )

cmake_minimum_required(VERSION 2.8)

#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(robocom_client_test)

set(WORKSPACE_ROOT "${PROJECT_SOURCE_DIR}/../../../../..")

# Allow access to all include files
include_directories("${WORKSPACE_ROOT}/src")

# Set location of files shared between arduino and the client
set(SHARED_SOURCE_DIR "${WORKSPACE_ROOT}/src/robocom/shared")
file(GLOB SHARED_SOURCE_FILES "${SHARED_SOURCE_DIR}/impl/*.cpp")
file(GLOB SHARED_MSG_SOURCE_FILES "${SHARED_SOURCE_DIR}/msg/impl/*.cpp")

set(robocom_client_test_BOARD uno)
set(robocom_client_test_PORT /dev/ttyACM0)
set(robocom_client_test_SRCS
 test.cpp
  ${SHARED_SOURCE_FILES}
  ${SHARED_MSG_SOURCE_FILES}
 )
generate_arduino_firmware(robocom_client_test)
