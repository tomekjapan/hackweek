add_executable(RoboComClientTester
  SerialPortTester.cpp
  main.cpp
  )

target_link_libraries(RoboComClientTester
  robocom_client
  robocom_shared
  UnitTest++
  )

include(ExternalProject)
ExternalProject_Add(arduino_test
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/client/test/arduino"
  INSTALL_COMMAND ""
  )
