cmake_minimum_required(VERSION 3.16)

# Set the target board
set(YAHAL_BOARD "rp2040-launchpad")
set(YAHAL_OPT_FLAGS -O0)
set(YAHAL_DIR "../YAHAL")
set(YAHAL_USE_NANO_SPECS 0)

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(game C CXX ASM)

add_executable(game
        game.cpp
        gui/GameGUI.h
        gui/GameGUI.cpp

        Enums.h
        gui/gamestate/GridUI.h
        gui/gamestate/GridUI.cpp
        gui/technical/Touch.h
        gui/technical/Touch.cpp
        gui/menustate/Menu.h
        gui/menustate/Menu.cpp
        gui/technical/Interactable.h

        logic/Field.cpp
        logic/Field.h
        logic/Map.cpp
        logic/Map.h
)

# Add YAHAL to target
yahal_add_me(game)
yahal_add_library(game uGUI)

target_include_directories(game
    PRIVATE .
)

yahal_add_custom_targets(game)
yahal_add_extra_outputs(game)

