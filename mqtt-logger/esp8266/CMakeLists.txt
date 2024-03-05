cmake_minimum_required(VERSION 3.16)

# Set the target board
set(YAHAL_BOARD "wifitick")

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(uart_esp8266)

add_executable(uart_esp8266 Communication.cpp Communication.h config.h)

# Add YAHAL library
yahal_add_me(uart_esp8266)

# Add Arduino libraries
target_link_arduino_libraries(uart_esp8266 PRIVATE core)

# This upload target
target_enable_arduino_upload(uart_esp8266)