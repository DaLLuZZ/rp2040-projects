cmake_minimum_required(VERSION 3.16)

# Set the target board
set(YAHAL_BOARD "rp2040-launchpad")
set(YAHAL_OPT_FLAGS -Ofast)

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(WifiTick-Programmer)

add_executable(WifiTick-Programmer WifiTick-Programmer.cpp)
 
# Add YAHAL to target
yahal_add_me(WifiTick-Programmer)
yahal_add_library(WifiTick-Programmer tinyUSB++)

yahal_add_custom_targets(WifiTick-Programmer)
yahal_add_extra_outputs(WifiTick-Programmer)

