cmake_minimum_required(VERSION 3.16)

# Set the target board
set(YAHAL_BOARD "wifitick")

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(MCT_esp8266)

add_executable(MCT_esp8266
        MCT_esp8266.cpp
        NTPClient.cpp
)
# Add YAHAL library
yahal_add_me(MCT_esp8266)

# Add Arduino libraries
target_link_arduino_libraries(MCT_esp8266 PRIVATE core)

# This upload target
target_enable_arduino_upload(MCT_esp8266)
