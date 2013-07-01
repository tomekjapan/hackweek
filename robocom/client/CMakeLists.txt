cmake_minimum_required(VERSION 2.6)
project(RoboComClient)

set(WORKSPACE_ROOT "${PROJECT_SOURCE_DIR}/../../..")

# Add the root source directory to the include path. Included
# headers are referenced with the whole path.
include_directories("${WORKSPACE_ROOT}/src")

# Enable all warnings and treat them as errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Library sources
add_library(robocomc
  Handle.cpp
  SerialPort.cpp
  )

# Unit tests
add_subdirectory(test)
enable_testing()
add_test(NAME RoboComClientTester COMMAND RoboComClientTester)
