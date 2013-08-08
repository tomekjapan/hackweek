set(CMAKE_TOOLCHAIN_FILE
  ../../../../src/3rdparty/arduino-cmake/cmake/ArduinoToolchain.cmake
  )

cmake_minimum_required(VERSION 2.8)

#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(robocom_arduino)

set(WORKSPACE_ROOT "${PROJECT_SOURCE_DIR}/../..")

# Allow access to all include files
include_directories("${WORKSPACE_ROOT}")
include_directories("${PROJECT_SOURCE_DIR}/libraries/Wire")
include_directories("${PROJECT_SOURCE_DIR}/libraries/Wire/utility")
include_directories("${PROJECT_SOURCE_DIR}/libraries/I2Cdev")
include_directories("${PROJECT_SOURCE_DIR}/libraries/MPU6050")

# Set location of files shared between arduino and the client
set(SHARED_SOURCE_DIR "${WORKSPACE_ROOT}/robocom/shared")
file(GLOB SHARED_SOURCE_FILES "${SHARED_SOURCE_DIR}/impl/*.cpp")
file(GLOB SHARED_MSG_SOURCE_FILES "${SHARED_SOURCE_DIR}/msg/impl/*.cpp")

# Set location of library sources
file(GLOB LIB_WIRE_SOURCE_FILES "${PROJECT_SOURCE_DIR}/libraries/Wire/*.cpp")
file(GLOB LIB_WIRE_C_SOURCE_FILES "${PROJECT_SOURCE_DIR}/libraries/Wire/*.c")
file(GLOB LIB_I2CDEV_SOURCE_FILES "${PROJECT_SOURCE_DIR}/libraries/I2Cdev/*.cpp")
file(GLOB LIB_MPU6050_SOURCE_FILES "${PROJECT_SOURCE_DIR}/libraries/MPU6050/*.cpp")

set(robocom_arduino_BOARD uno)
set(robocom_arduino_PORT /dev/ttyACM0)

# Include shared sources directly instead of building them
# as a library.
set(robocom_arduino_SRCS
  impl/RobotServer.cpp
  impl/Encoder.cpp
  impl/Motor.cpp
  impl/Gyro.cpp
  impl/Servo.cpp
  impl/main.cpp
  ${SHARED_SOURCE_FILES}
  ${SHARED_MSG_SOURCE_FILES}
  ${LIB_WIRE_SOURCE_FILES}
  ${LIB_WIRE_C_SOURCE_FILES}
  ${LIB_I2CDEV_SOURCE_FILES}
  ${LIB_MPU6050_SOURCE_FILES}
)
generate_arduino_firmware(robocom_arduino)
