cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME f401_coremark)

# Include toolchain file
#set(USE_CLANG TRUE)

if(USE_CLANG)
    message(STATUS "Using Clang toolchain config")
    include("cmake/llvm-for-arm.cmake")
else()
    message(STATUS "Using GCC toolchain config")
    include("cmake/gcc-arm-none-eabi.cmake")
endif()

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
        coremark
)

if (USE_CLANG)
    set(PRINTF_PORT "printf_clang_port.c")
else ()
    set(PRINTF_PORT "printf_gcc_port.c")
endif ()

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
        coremark/core_list_join.c
        coremark/core_matrix.c
        coremark/core_portme.c
        coremark/core_state.c
        coremark/core_util.c
        coremark/core_main.c
        coremark/${PRINTF_PORT}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
        ITERATIONS=1000
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
