# Library sources
add_library(robocom_shared
  impl/Message.cpp
  impl/MessageIO.cpp
  impl/MessagePool.cpp
  impl/MessageQueue.cpp
  impl/Server.cpp
  msg/impl/FlushResponse.cpp
  msg/impl/SetWheelDriveRequest.cpp
  msg/impl/WheelDriveChangedNotice.cpp
  msg/impl/EncoderReadingRequest.cpp
  )

##########################################################
# Tests

add_subdirectory(test)

add_test(NAME RoboComSharedTester COMMAND RoboComSharedTester)
