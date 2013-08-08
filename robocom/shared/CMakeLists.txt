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
  msg/impl/EncoderReadingNotice.cpp
  msg/impl/GyroReadingRequest.cpp
  msg/impl/GyroReadingNotice.cpp
  )

##########################################################
# Tests

