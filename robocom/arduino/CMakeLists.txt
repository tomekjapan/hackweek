set(CMAKE_TOOLCHAIN_FILE
  ../../../arduino-cmake/cmake/ArduinoToolchain.cmake
  )

cmake_minimum_required(VERSION 2.8)

#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(robocom_arduino)

set(WORKSPACE_ROOT "${PROJECT_SOURCE_DIR}/../..")

# Allow access to all include files
include_directories("${WORKSPACE_ROOT}")

# Set location of files shared between arduino and the client
set(SHARED_SOURCE_DIR "${WORKSPACE_ROOT}/robocom/shared")
file(GLOB SHARED_SOURCE_FILES "${SHARED_SOURCE_DIR}/impl/*.cpp")
file(GLOB SHARED_MSG_SOURCE_FILES "${SHARED_SOURCE_DIR}/msg/impl/*.cpp")

set(robocom_arduino_BOARD uno)
set(robocom_arduino_PORT /dev/tty.usbmodem1421)

# Include shared sources directly instead of building them
# as a library.
set(robocom_arduino_SRCS
  impl/RobotServer.cpp
  impl/Encoder.cpp
  impl/Motor.cpp
  impl/main.cpp
  ${SHARED_SOURCE_FILES}
  ${SHARED_MSG_SOURCE_FILES}
)
generate_arduino_firmware(robocom_arduino)
