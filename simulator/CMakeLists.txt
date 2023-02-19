cmake_minimum_required(VERSION 3.5)

# Create the TkArch Project
project(TkArch VERSION 0.0.1)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Debug build options
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  # Define DEBUG
  # add_definitions(-DDEBUG)

  # Memory leak detection

  # Verbose makefiles/cmake
  # set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# Use shared libraries
set(BUILD_SHARED_LIBS ON)
set(BUILD_TESTS OFF)

# 3rd party libraries
add_subdirectory(lib/gtest)

# Add to include path
include_directories(include)

# Libraries
add_subdirectory(src/types)
add_subdirectory(src/meta)

add_subdirectory(src/observers)
add_subdirectory(src/events)
add_subdirectory(src/util)

add_subdirectory(src/basic)
add_subdirectory(src/compound)
add_subdirectory(src/complex)
add_subdirectory(src/components)

add_subdirectory(src/core)

# Applications
add_subdirectory(apps/app)