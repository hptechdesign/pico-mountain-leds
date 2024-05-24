# Generated Cmake Pico project file

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

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(PICO_BOARD pico_w)
set(PICO_PLATFORM rp2040)
set(PICO_COMPILER pico_arm_gcc)
set(PROJECT_NAME picow_hello_world)
set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})

# Pull in Raspberry Pi Pico SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Print the PIMORONI_LIB_PATH to verify it's set correctly
message(STATUS "PIMORONI_LIB_PATH is set to: ${PIMORONI_LIB_PATH}")

# Set the project name
project(NurseryLights C CXX ASM)


add_compile_options(-Wall
    -Wno-format          # int != int32_t 
    -Wno-unused-function # we have some for the docs that aren't called
    -Wno-maybe-uninitialized
)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(${PROJECT_NAME}
   ${PROJECT_NAME}.cpp
   )



# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
                                            ${PIMORONI_LIB_PATH}
                                            ${PIMORONI_LIB_PATH}/drivers/plasma
                                            ${PIMORONI_LIB_PATH}/libraries/plasma_stick
                                            
                                                   )

include(${PIMORONI_LIB_PATH}/libraries/plasma_stick/plasma_stick.cmake)
#pico_generate_pio_header(${PROJECT_NAME} ${PIMORONI_LIB_PATH}/drivers/plasma/ws2812.pio)
target_link_libraries(${PROJECT_NAME}
   pico_stdlib              # for core functionality
   pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else
   hardware_pio
   plasma_stick
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)