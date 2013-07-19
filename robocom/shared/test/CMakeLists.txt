add_executable(RoboComSharedTester
  MessageTester.cpp
  MessagePoolTester.cpp
  MessageQueueTester.cpp
  main.cpp
  )

target_link_libraries(RoboComSharedTester
  robocom_shared
  UnitTest++
  )
