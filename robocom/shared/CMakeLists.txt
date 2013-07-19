# Library sources
add_library(robocom_shared
  impl/Message.cpp
  impl/MessagePool.cpp
  impl/MessageQueue.cpp
  msg/impl/FlushResponse.cpp
  msg/impl/SetWheelDriveRequest.cpp
  msg/impl/WheelDriveChangedNotice.cpp
  )

##########################################################
# Tests

add_subdirectory(test)

add_test(NAME RoboComSharedTester COMMAND RoboComSharedTester)
