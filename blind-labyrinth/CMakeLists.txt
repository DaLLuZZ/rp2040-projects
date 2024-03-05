cmake_minimum_required(VERSION 3.16)

set(CMAKE_BUILD_TYPE DEBUG)
#set(CMAKE_BUILD_TYPE RELEASE)

# Set the target board
set(YAHAL_BOARD "rp2040-launchpad")

# Locate the YAHAL library folder
include(YAHAL_import.cmake)
project(labyrinth C CXX ASM)

add_executable(labyrinth
        labyrinth.cpp
        gameMaster.h
        main.cpp
        visualizer.h
        visual_helpers/charpattern.h
        visual_helpers/visual_items.h
        visual_helpers/pixel_stream_rgb565_const.h
        SDTools/sdWriter.h

)

# Add YAHAL to target
yahal_add_me(labyrinth)
yahal_add_library(labyrinth FatFs)


yahal_add_custom_targets(labyrinth)
yahal_add_extra_outputs(labyrinth)

