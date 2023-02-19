# ############################################################
# Setup
# ############################################################

# Set the name of the module
set(MODULE_NAME GTest)

# Set the list of the module's dependencies in the project
set(MODULE_DEPS
)

# If this module is a test module, set the list of the test units
set(MODULE_TEST_UNITS
)

if(NOT BUILD_TESTS)
  return()
endif()

# Custom commands before library creation
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY
  https://github.com/google/googletest.git
  GIT_TAG
  v1.13.0
)

FetchContent_MakeAvailable(googletest)

# Setup the module using the generic setup script
include(${CMAKE_SOURCE_DIR}/cmake/generic.cmake)

target_link_libraries(${PROJECT_NAME}
  GTest::gtest_main
  GTest::gmock_main
)
