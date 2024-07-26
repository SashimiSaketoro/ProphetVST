cmake_minimum_required(VERSION 3.14)
project(Prophet6VST VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add VST3 SDK path
set(VST3_SDK_ROOT "" CACHE PATH "Path to the VST3 SDK")
if(NOT VST3_SDK_ROOT)
    message(FATAL_ERROR "Please specify the path to the VST3 SDK using -DVST3_SDK_ROOT=<path>")
endif()

include_directories(${VST3_SDK_ROOT})

# Add JUCE path if using JUCE for GUI
set(JUCE_PATH "" CACHE PATH "Path to JUCE")
if(JUCE_PATH)
    add_subdirectory(${JUCE_PATH} JUCE)
    include_directories(${JUCE_PATH}/modules)
endif()

# Prophet 6 VST3 Plugin
set(PROPHET6_VST3_SOURCES
    src/Prophet6Processor.cpp
    src/Prophet6Controller.cpp
    src/Prophet6Plugin.cpp
    src/Prophet6AudioProcessorEditor.cpp
    src/oscillator.cpp
    src/filter.cpp
    src/amplifier.cpp
    src/modulation.cpp
    src/arpeggiator.cpp
    src/sequencer.cpp
    src/effects.cpp
)

add_library(Prophet6VST MODULE ${PROPHET6_VST3_SOURCES})

target_include_directories(Prophet6VST PRIVATE 
    ${VST3_SDK_ROOT}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link against VST3 SDK libraries
target_link_libraries(Prophet6VST PRIVATE 
    sdk
    base
    pluginterfaces
)

# If using JUCE for GUI
if(JUCE_PATH)
    target_link_libraries(Prophet6VST PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
    )
endif()

# Set output directory
set_target_properties(Prophet6VST PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/VST3"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/VST3"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/VST3"
)

# Install target
install(TARGETS Prophet6VST
    LIBRARY DESTINATION "VST3"
)

# JUCE Library
juce_add_plugin(Prophet6VST
    PLUGIN_MANUFACTURER_CODE Yoco
    PLUGIN_CODE P6vs
    FORMATS VST3 Standalone
    PRODUCT_NAME "Prophet 6"
)

target_compile_features(Prophet6VST PRIVATE cxx_std_17)

target_compile_definitions(Prophet6VST
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(Prophet6VST
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Standalone settings
juce_generate_juce_header(Prophet6VST)

# Apple Silicon support
if(APPLE)
    set_target_properties(Prophet6VST PROPERTIES
        OSX_ARCHITECTURES "x86_64;arm64"
    )
endif()