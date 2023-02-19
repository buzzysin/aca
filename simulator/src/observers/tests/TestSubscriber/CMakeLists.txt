# ############################################################
# Setup
# ############################################################

# Set the name of the module
set(MODULE_NAME Subscriber)

# Set the list of the module's dependencies in the project
set(MODULE_DEPS
  Observers
)

# If this module is a test module, set the list of the test units
set(MODULE_TEST_UNITS
  SimpleObserver
  SimpleTypedObserver
)

# Setup the module using the generic setup script
include(${CMAKE_SOURCE_DIR}/cmake/generic.cmake)