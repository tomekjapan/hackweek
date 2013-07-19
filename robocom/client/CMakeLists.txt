# Library sources
add_library(robocom_client
  impl/Handle.cpp
  impl/SerialPort.cpp
  )

##########################################################
# Tests

add_subdirectory(test)

# First build an upload arduino firmware which will talk
# to our unit test over the serial port
add_test(ArduinoBuildFirmware
  make -C test/arduino_test-prefix/src/arduino_test-build )
add_test(ArduinoUpload
  make -C test/arduino_test-prefix/src/arduino_test-build upload )

# Now the actal tests
add_test(NAME RoboComClientTester COMMAND RoboComClientTester)
