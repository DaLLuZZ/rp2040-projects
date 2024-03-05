cmake_minimum_required(VERSION 3.16)

# Set the target board
set(YAHAL_BOARD "rp2040-launchpad")
set(YAHAL_USE_NANO_SPECS 0)

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(RoomMeasurements C CXX ASM)

add_executable(RoomMeasurements Room.cpp Room.h main.cpp)

# Add YAHAL
yahal_add_me(RoomMeasurements)
yahal_add_library(RoomMeasurements uGUI)

yahal_add_custom_targets(RoomMeasurements)
yahal_add_extra_outputs(RoomMeasurements)
