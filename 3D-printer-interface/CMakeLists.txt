cmake_minimum_required(VERSION 3.16)

# Set the target board
set(YAHAL_BOARD "wifitick")

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(web_server)

add_executable(web_server
    web_server.cpp
)

# Add Arduino libraries
target_link_arduino_libraries(web_server PRIVATE core)

# This upload target
target_enable_arduino_upload(web_server)

