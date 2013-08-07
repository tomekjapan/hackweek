add_executable(FirstSample
  first.cpp
  )

target_link_libraries(FirstSample
  robocom_client
  robocom_shared
  )

