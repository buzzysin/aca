# #######################################
# Generic file for creating a new module
# #######################################

# Passed in variables:
# <MODULE_NAME>
# - The name of the module
#
# [MODULE_DEPS...]
# - A list of dependencies for this module
#
# [MODULE_TEST_UNITS...]
# - If this is a test module, then this is a list of
# unit tests to be added to the test module (submodules)

# Useful for later:

# Get the name of the directory this file is in
get_filename_component(MODULE_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# ###############################################
# Start
# ###############################################

# The name of the module needs to be passed in
if(NOT MODULE_NAME)
  message(FATAL_ERROR "MODULE_NAME not defined")
  return()
endif()

# Detect if this is a library module. It is a library if it has a src directory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src)
  set(MODULE_IS_LIB 1)
else()
  set(MODULE_IS_LIB 0)
endif()

# Detect if this is an executable module. It is an executable if it has a
# main.cpp file in the src directory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
  set(MODULE_IS_EXE 1)
else()
  set(MODULE_IS_EXE 0)
endif()

# Detect if this is a test submodule. It is a test submodule if the
# directory name of this CMakelists.txt file is "tests"
if(MODULE_DIR_NAME STREQUAL "tests")
  set(MODULE_IS_TEST 1)
else()
  set(MODULE_IS_TEST 0)
endif()

# Detect if this is a unit test submodule. It is a unit test submodule if the directory
# name of this CMakelists.txt starts with "Test" (this project's convention)
if(MODULE_DIR_NAME MATCHES "^Test")
  set(MODULE_IS_TEST_UNIT 1)
else()
  set(MODULE_IS_TEST_UNIT 0)
endif()

# Detect if this module has a test submodule. It has a test submodule
# if it has a "tests" directory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
  set(MODULE_HAS_TESTS 1)
else()
  set(MODULE_HAS_TESTS 0)
endif()

# ###############################################################
# Conflicts
# ###############################################################
if(MODULE_IS_LIB AND MODULE_IS_EXE)
  # If this is an executable, but a library was detected, then
  # the executable takes precedence
  set(MODULE_IS_LIB 0)
elseif(MODULE_IS_LIB AND MODULE_IS_TEST)
  # If this is a test module, but a library was detected, then
  # the test module takes precedence
  set(MODULE_IS_LIB 0)
elseif(MODULE_IS_LIB AND MODULE_IS_EXE)
  # If this is an executable, but a library was detected, then
  # the executable takes precedence
  set(MODULE_IS_LIB 0)
elseif(MODULE_IS_LIB AND MODULE_IS_TEST_UNIT)
  # If this is a unit test, but a library was detected, then
  # the unit test takes precedence
  set(MODULE_IS_LIB 0)
endif()

if (MODULE_IS_TEST OR MODULE_IS_TEST_UNIT)

  # If this is a test module, but tests are not being built, then
  # skip this module
  if(NOT BUILD_TESTS)
    message(STATUS "${PROJECT_NAME} :: Skipping ${PROJECT_NAME}'s tests because BUILD_TESTS is OFF")
    return()
  endif()
endif()

# ###############################################################
# Preamble
# ###############################################################

# Create the project
if(MODULE_IS_LIB OR MODULE_IS_EXE)
  # Libraries and executables will have the name ${CMAKE_PROJECT_NAME}${MODULE_NAME}
  # i.e. "TkArch""Module"
  project(${CMAKE_PROJECT_NAME}${MODULE_NAME})
elseif(MODULE_IS_TEST)
  # Test modules will have the name ${CMAKE_PROJECT_NAME}Test${MODULE_NAME}
  # i.e. "TkArchTest""Module"
  project(${CMAKE_PROJECT_NAME}Test${MODULE_NAME})
elseif(MODULE_IS_TEST_UNIT)
  # Test module will extend the parent module name with their own name
  # i.e. "TkArch""Module""<Name>"
  # Since they are submodules of the test module, they will be
  # named "TkArchTestModuleName"
  project(${PROJECT_NAME}${MODULE_NAME})
else()
  message(FATAL_ERROR "Could not determine type of module")
endif()

# required for gtest and some cmake features like target_link_libraries
cmake_minimum_required(VERSION 3.14)

# Minumum C++ standard is 14 for gtest, but we want to use 20 for some features
set(CMAKE_CXX_STANDARD 20)

message(STATUS "${PROJECT_NAME} :: Configuring ${PROJECT_NAME}")

# If this module is a library, we need to...
if(MODULE_IS_LIB)
  # Collect all the source files in the src directory
  message(STATUS "${PROJECT_NAME} :: Collecting source files for ${PROJECT_NAME}")
  file(GLOB_RECURSE MODULE_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

  # Create the library target
  message(STATUS "${PROJECT_NAME} :: Creating library target ${PROJECT_NAME}")
  add_library(${PROJECT_NAME}
    ${MODULE_SRC_FILES}
  )

  # Create an alias for the library target so it is referenceable
  # from other modules
  message(STATUS "${PROJECT_NAME} :: Creating alias ${CMAKE_PROJECT_NAME}::${MODULE_NAME}")
  add_library(${CMAKE_PROJECT_NAME}::${MODULE_NAME} ALIAS ${PROJECT_NAME})

  # Add the src directory to the include directories
  message(STATUS "${PROJECT_NAME} :: Adding ${CMAKE_CURRENT_SOURCE_DIR}/src to include directories")
  target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

elseif(MODULE_IS_EXE)
  # Create the executable target
  message(STATUS "${PROJECT_NAME} :: Creating executable target ${PROJECT_NAME}")
  file(GLOB_RECURSE MODULE_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

  add_executable(${PROJECT_NAME}
    ${MODULE_SRC_FILES}
  )

  # We don't need to create an alias for the executable target (probably)

  # Add the src directory to the include directories
  message(STATUS "${PROJECT_NAME} :: Adding ${CMAKE_CURRENT_SOURCE_DIR}/src to include directories")
  target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
elseif(MODULE_IS_TEST)
  # The test module will contain further submodules, so we should use the declared
  # MODULE_TEST_UNITS list to create the submodules
  message(STATUS "${PROJECT_NAME} :: Creating test target ${PROJECT_NAME}")

  if(MODULE_TEST_UNITS)
    foreach(MODULE_TEST_UNIT ${MODULE_TEST_UNITS})
      message(STATUS "${PROJECT_NAME} :: Creating unit test ${MODULE_TEST_UNIT}")

      # Unit tests always start with "Test", so a Clock unit test will be
      # named "TestClock"
      add_subdirectory(Test${MODULE_TEST_UNIT})
    endforeach()
  else()
    # If no unit tests are found, we should probably let the user know
    message(WARNING "${PROJECT_NAME} :: No unit tests found")
  endif()
elseif(MODULE_IS_TEST_UNIT)
  # Unit tests will be executable targets, so we should create them
  # as such
  message(STATUS "${PROJECT_NAME} :: Creating unit test target ${PROJECT_NAME}")

  # We need to add the the project GTest dependency to the unit test
  # target. Append GTest to the MODULE_DEPS list
  list(APPEND MODULE_DEPS GTest)

  # Collect all the source files in the src directory
  file(GLOB_RECURSE MODULE_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

  # Create the executable target
  add_executable(${PROJECT_NAME}
    ${MODULE_IS_EXEC_MAIN}
    ${MODULE_SRC_FILES}
  )
else()
  message(FATAL_ERROR "${PROJECT_NAME} :: Could not determine type of module")
  return()
endif()

# Independent of target type:

# Link dependencies if they are provided in MODULE_DEPS list
if(MODULE_DEPS)
  message(STATUS "${PROJECT_NAME} :: Found depenencies for ${PROJECT_NAME}")

  foreach(MODULE_DEP ${MODULE_DEPS})
    message(STATUS "${PROJECT_NAME} ::         Linking dependency ${MODULE_DEP}")
    target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}::${MODULE_DEP})
  endforeach()
endif()

# Set include directories for the module
message(STATUS "${PROJECT_NAME} :: Adding relevant include directories")

# Usually libraries have a test project:
if(MODULE_IS_LIB AND MODULE_HAS_TESTS)
  # Add the test directory to the include directories
  message(STATUS "${PROJECT_NAME} :: Found a test project for ${PROJECT_NAME}")
  add_subdirectory(tests)
endif()

# Test modules don't need to include the src directory,
# since they contain no source files
if(NOT MODULE_IS_TEST)
  target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()

if(MODULE_IS_TEST_UNIT)
  # Unit tests need to enable testing
  enable_testing()

  include(GoogleTest)
  gtest_discover_tests(${PROJECT_NAME})
endif()

# ###############################################################
# Postamble
# ###############################################################

# Finished
message(STATUS "${PROJECT_NAME} :: Finished configuring ${PROJECT_NAME}")