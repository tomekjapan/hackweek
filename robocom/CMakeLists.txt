cmake_minimum_required(VERSION 2.8)
project(RoboComClient)

set(WORKSPACE_ROOT "${PROJECT_SOURCE_DIR}/..")

# Add the root source directory to the include path. Included
# headers are referenced with the whole path.
include_directories("${WORKSPACE_ROOT}")

# Set location of files shared between arduino and the client
set(SHARED_SOURCE_DIR "${WORKSPACE_ROOT}/src/robocom/shared")

# Enable all warnings and treat them as errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

enable_testing()

add_subdirectory(shared)
add_subdirectory(client)
add_subdirectory(samples)
