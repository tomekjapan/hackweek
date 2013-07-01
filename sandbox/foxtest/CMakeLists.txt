cmake_minimum_required(VERSION 2.6)
project(foxtest)

set(WORKSPACE_ROOT "${PROJECT_SOURCE_DIR}/../../..")

include_directories("${WORKSPACE_ROOT}/include/fox-1.7")
link_directories("${WORKSPACE_ROOT}/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

add_executable(foxtest main.cpp)

target_link_libraries(foxtest FOX-1.7)
