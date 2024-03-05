cmake_minimum_required(VERSION 3.16)

#set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_BUILD_TYPE RELEASE)

# Set the target board
set(YAHAL_BOARD "wifitick")
set(YAHAL_DIR "../../YAHAL")

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(ESP_Project)

add_executable(ESP_Project
        ESP.cpp
)
# Add YAHAL library
yahal_add_me(ESP_Project)

# Add Arduino libraries
target_link_arduino_libraries(ESP_Project PRIVATE core)

# This upload target
target_enable_arduino_upload(ESP_Project)


