cmake_minimum_required(VERSION 3.16)

# Set the target board
set(YAHAL_BOARD "rp2040-launchpad")
# set(YAHAL_DIR "/home/steffen/Uebungen/Microcontrollertechnik/YAHAL")
set(YAHAL_OPT_FLAGS -O0)

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(Implementation)

add_executable(Implementation
        main.cpp
        Helper.cpp
        Sensors/dht.cpp
        Hardware/OnOffSystem.cpp
        Hardware/HumiditySensor.cpp
        Hardware/MoistureSensor.cpp
        Hardware/TemperatureSensor.cpp
        MinMaxController.cpp
        Hardware/SDCard.cpp
        Hardware/ISensor.cpp
        Hardware/Button.cpp
        UartController.cpp
        DataLogger.cpp
        Greenhouse.cpp
)

# Add YAHAL to target
yahal_add_me(Implementation)
yahal_add_custom_targets(Implementation)
yahal_add_extra_outputs(Implementation)
yahal_add_library(Implementation FatFs)

# target_compile_options(Implementation PRIVATE -frtti)