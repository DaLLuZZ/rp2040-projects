cmake_minimum_required(VERSION 3.16)

#set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_BUILD_TYPE RELEASE)

# Set the target board
set(YAHAL_BOARD "rp2040-launchpad")

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(MCT_Project_GS C CXX ASM)

add_executable(MCT_Project_GS
        ChessMain.cpp
        Color.cpp
        Color.h
        ChessPieces.cpp
        ChessPieces.h
        Menu.cpp
        Menu.h
        json.hpp
        ../ESP_Project/creds.h
)

# Add YAHAL to target
yahal_add_me(MCT_Project_GS)
yahal_add_library(MCT_Project_GS uGUI)

yahal_add_custom_targets(MCT_Project_GS)
yahal_add_extra_outputs(MCT_Project_GS)

