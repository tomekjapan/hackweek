add_executable(RoboComClientTester
  SerialPortTester.cpp
  main.cpp
  )

target_link_libraries(RoboComClientTester UnitTest++)
