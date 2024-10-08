# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

# Set C++ standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Check if the environment variables are set
if (NOT DEFINED ENV{PICO_SDK_PATH})
    message(FATAL_ERROR "PICO_SDK_PATH environment variable is not set")
endif()

if (NOT DEFINED ENV{PIMORONI_LIB_PATH})
    message(FATAL_ERROR "PIMORONI_LIB_PATH environment variable is not set")
endif()

# Use the environment variables
set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(PIMORONI_LIB_PATH $ENV{PIMORONI_LIB_PATH})

set(CMAKE_BUILD_TYPE Debug)
set(PICO_BOARD pico_w)
set(PICO_PLATFORM rp2040)
set(PICO_COMPILER pico_arm_gcc)
set(PROJECT_NAME NurseryLights)

# Pull in Raspberry Pi Pico SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Print the PIMORONI_LIB_PATH to verify it's set correctly
message(STATUS "PIMORONI_LIB_PATH is set to: ${PIMORONI_LIB_PATH}")

# Set the project name
project(NurseryLights C CXX ASM)

# Enable assembly language
enable_language(ASM)

# Add compile options
add_compile_options(-Wall
    -Wno-format          # int != int32_t 
    -Wno-unused-function # we have some for the docs that aren't called
    -Wno-maybe-uninitialized
)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable
add_executable(${PROJECT_NAME}
   main.cpp
   command_parser.cpp
   led_controller.cpp
)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PIMORONI_LIB_PATH}
    ${PIMORONI_LIB_PATH}/drivers/plasma
    ${PIMORONI_LIB_PATH}/libraries/plasma_stick
    ${PICO_SDK_PATH}/src/common/pico_stdlib/include # Include for standard library
    ${PICO_SDK_PATH}/src/common/hardware_gpio/include  # Include for GPIO functions
)

# Include the plasma stick library CMake file
include(${PIMORONI_LIB_PATH}/libraries/plasma_stick/plasma_stick.cmake)

# Link libraries
target_link_libraries(${PROJECT_NAME}
   pico_stdlib              # for core functionality
   pico_cyw43_arch_none     # for WiFi support
   hardware_pio             # for PIO
   plasma_stick             # for plasma stick functionality
)

# Create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

# Enable USB and UART output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
