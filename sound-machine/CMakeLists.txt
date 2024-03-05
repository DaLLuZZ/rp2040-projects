cmake_minimum_required(VERSION 3.16)

set(CMAKE_BUILD_TYPE DEBUG)

# Set the target board
set(YAHAL_BOARD "rp2040-launchpad")

set(YAHAL_DIR "${CMAKE_SOURCE_DIR}/../YAHAL")

include(YAHAL_import.cmake)

#project(mct C CXX ASM)
project(mct)

add_executable(mct
        src/main.cpp
        mp3/achievement-win-drum.cpp
        mp3/angelic-drum.cpp
        mp3/bad-joke-drum.cpp
        mp3/bass-futuristic.cpp
        mp3/bass-mystirious.cpp
        mp3/bass-pulsating.cpp
        mp3/bass-rumble-hum.cpp
        mp3/cinematic-synth-riser.cpp
        mp3/deep-dark-horror-drum.cpp
        mp3/electric-guitar.cpp
        mp3/guitar-stroke-down-slow.cpp
        mp3/guitar-stroke-up-slow.cpp
        mp3/melodic-guitar.cpp
        mp3/synth-intense-rhythm.cpp
        mp3/synth-mechanical-notification.cpp
        mp3/synth-pass.cpp
)

## Add YAHAL to target
yahal_add_me(mct)


yahal_add_library(mct uGUI)

# Generate cpp-file from mp3 files
#if (WIN32 OR UNIX AND NOT APPLE)
    mp3pcm_to_cpp(mct
            sounds/achievement-win-drum.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/angelic-drum.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/bad-joke-drum.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/bass-futuristic.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/bass-mystirious.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/bass-pulsating.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/bass-rumble-hum.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/cinematic-synth-riser.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/deep-dark-horror-drum.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/electric-guitar.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/guitar-stroke-down-slow.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/guitar-stroke-up-slow.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/melodic-guitar.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/synth-intense-rhythm.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/synth-mechanical-notification.mp3
            OUTPUT_DIR mp3
    )
    mp3pcm_to_cpp(mct
            sounds/synth-pass.mp3
            OUTPUT_DIR mp3
    )
#endif ()

yahal_add_custom_targets(mct)
yahal_add_extra_outputs(mct)
