cmake_minimum_required(VERSION 3.16)

# Set the target board
set(YAHAL_BOARD "rp2040-launchpad")

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(MCT_rp2040)

add_executable(MCT_rp2040
        MCT_rp2040.cpp
        Display.cpp
        PlaySound.cpp
        Stoppuhr.cpp
        tiny-json.c
        gen/pfeil-rechts.cpp
        gen/pfeil-links.cpp
        gen/pfeil-hoch.cpp
        gen/pfeil-runter.cpp
)

# Add YAHAL to target
yahal_add_me(MCT_rp2040)
# Add the uGUI library
yahal_add_library(MCT_rp2040 uGUI)

# Generate cpp-file from png
png_to_cpp(MCT_rp2040
        png/pfeil-rechts.png
        BG_COLOR    0x000000
        OUTPUT_FILE gen/pfeil-rechts.cpp)
png_to_cpp(MCT_rp2040
        png/pfeil-links.png
        BG_COLOR    0x000000
        OUTPUT_FILE gen/pfeil-links.cpp)
png_to_cpp(MCT_rp2040
        png/pfeil-hoch.png
        BG_COLOR    0x000000
        OUTPUT_FILE gen/pfeil-hoch.cpp)
png_to_cpp(MCT_rp2040
        png/pfeil-runter.png
        BG_COLOR    0x000000
        OUTPUT_FILE gen/pfeil-runter.cpp)

yahal_add_custom_targets(MCT_rp2040)
yahal_add_extra_outputs(MCT_rp2040)
