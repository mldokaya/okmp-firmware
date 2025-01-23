cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Set the project name
set(CMAKE_PROJECT_NAME omp_firmware)

if(CMAKE_CROSSCOMPILING)
    # Include toolchain file
    include("cmake/gcc-arm-none-eabi.cmake")
endif()

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

if(CMAKE_CROSSCOMPILING)
    include(CMakePrintHelpers)
    cmake_print_variables(CMAKE_C_COMPILER)
    # Create an executable object type
    add_executable(${CMAKE_PROJECT_NAME})
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE DEBUG_DEFAULT_INTERRUPT_HANDLERS)

    # Add STM32CubeMX generated sources
    add_subdirectory(external)

    # Link directories setup
    target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined library search paths
    )

    # Add sources to executable
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        src/main.c
        src/freertos/freertos.c
        src/input.c
        src/led.c
        src/is31fl3731/is31fl3731.c
        src/sh1106/sh1106.c
        src/gui.c
        src/display.c
        external/tinyusb/src/tusb.c
        external/tinyusb/src/class/hid/hid_device.c
        external/tinyusb/src/class/hid/hid_host.c
        external/tinyusb/src/common/tusb_fifo.c
        external/tinyusb/src/device/usbd_control.c
        external/tinyusb/src/device/usbd.c
        external/tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.c
        external/tinyusb/src/portable/synopsys/dwc2/dwc2_common.c
        src/usb/usb_descriptors.c
        src/stm32f411/gpio.c
        src/stm32f411/spi.c
        src/stm32f411/i2c.c
    )
    # Add include paths
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined include paths
        src/stm32f411
        src/is31fl3731
        src/sh1106
        src/usb
        src/freertos
        src/
        external/
        external/tinyusb/src
        external/tinyusb/hw/bsp
    )

    # Add project symbols (macros)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined symbols
    )

    # Add linked libraries
    target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32-drivers
        # Add user defined libraries
    )
else()
    # TODO: Add unit tests
    include(CTest)
    add_library(Unity STATIC
        external/Unity/src/unity.c
    )
    target_include_directories(Unity PUBLIC
        external/Unity/src
    )
    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    enable_testing()
    add_subdirectory(tests)
endif()