# ############################################################
# Setup
# ############################################################

# Set the name of the module
set(MODULE_NAME Components)

# Set the list of the module's dependencies in the project
set(MODULE_DEPS
  Util
  Basic
  Compound
  Complex
)

# If this module is a test module, set the list of the test units
set(MODULE_TEST_UNITS
)

# Setup the module using the generic setup script
include(${CMAKE_SOURCE_DIR}/cmake/generic.cmake)