cmake_minimum_required(VERSION 3.16)

set(YAHAL_USE_NANO_SPECS 0)
set(CMAKE_BUILD_TYPE DEBUG)
#set(CMAKE_BUILD_TYPE RELEASE)

# Set the target board
set(YAHAL_BOARD "rp2040-launchpad")

# Locate the YAHAL library folder
include(YAHAL_import.cmake)

project(Mikrocontroller_Lichtorgel)

add_executable(Mikrocontroller_Lichtorgel
    lichtorgel.cpp
        src/global_constants.h
        src/view/ui_constants.h
        src/view/view_controller.h
        src/view/view_controller.cpp
        src/view/UI_components/ui_component.h
        src/view/UI_components/ui_component.cpp
        src/view/UI_components/fft_ui_components/fft_ui_component.cpp
        src/view/UI_components/leds_ui_components/leds_ui_component.cpp
        src/led_controller/leds.cpp
        src/audio_fft/audio_analyzer.h
        src/audio_fft/audio_analyzer.cpp
        src/view/UI_components/settings_ui_component/led_settings_bar.cpp
        src/view/UI_components/settings_ui_component/led_settings_bar.h
        src/view/UI_components/settings_ui_component/freq_color_bar.cpp
        src/view/UI_components/settings_ui_component/freq_color_bar.h
        src/view/UI_components/settings_ui_component/bar_ui_component_base_class/ui_bar_component.cpp
        src/view/UI_components/settings_ui_component/bar_ui_component_base_class/ui_bar_component.h
        src/user_controller/user_control.cpp
        src/user_controller/user_control.h
)

# Add YAHAL to target
yahal_add_me(Mikrocontroller_Lichtorgel)

yahal_add_library(Mikrocontroller_Lichtorgel CMSISDSP)
yahal_add_library(Mikrocontroller_Lichtorgel uGUI)

yahal_add_custom_targets(Mikrocontroller_Lichtorgel)
yahal_add_extra_outputs(Mikrocontroller_Lichtorgel)
