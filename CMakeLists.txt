cmake_minimum_required(VERSION 3.14.0)

project(TkArch VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS ON)

set(MODULE_NAME TkArch)

set(MODULE_SUBDIRS debug circuit isa signals)

set(MODULE_DEPS)

set(MODULE_APPS simulator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if ((CMAKE_BUILD_TYPE STREQUAL "Debug") OR (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
  message(STATUS "Debug mode")
  # add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  # add_link_options(-fsanitize=address -fno-omit-frame-pointer)
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
  add_compile_definitions(DEBUG_MODE=1)
  add_compile_definitions(PRINTER=1)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

include(${CMAKE_SOURCE_DIR}/cmake/config.cmake)
