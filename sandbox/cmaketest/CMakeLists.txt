cmake_minimum_required(VERSION 2.6)
project(Tutorial)

set(WORKSPACE_ROOT "${PROJECT_SOURCE_DIR}/../../..")

# The installation directory
set(CMAKE_INSTALL_PREFIX "${WORKSPACE_ROOT}/test-dist")

# The version number
set(Tutorial_VERSION_MAJOR 1)
set(Tutorial_VERSION_MINOR 0)

# Configure the header file to pass some of the CMake
# settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
	"${PROJECT_BINARY_DIR}/TutorialConfig.h"
)

# Add the root source directory to the include path. Included
# headers are referenced with the whole path.
include_directories("${WORKSPACE_ROOT}/src")

# Add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# Add a subdirectory with a library
add_subdirectory(../MathFunctions ../MathFunctions)

# Add the final executable
add_executable(Tutorial tutorial.cpp)

target_link_libraries(Tutorial MathFunctions)

install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h" DESTINATION include)
