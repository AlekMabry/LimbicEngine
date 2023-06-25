cmake_minimum_required(VERSION 3.26.0)

# Project and metadata
project(
    LimbicApplication
    VERSION 0.1.0
    DESCRIPTION "VR Game, Game Engine, and Editor"
    LANGUAGES CXX
    )

# Project-wide compiler and linker settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable folders in IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Append 'd' to executables/libraries compiled in debug
# set_target_properties(GLOBAL PROPERTIES DEBUG_POSTFIX d)

add_compile_options(
    /W4         # Warning level 4
    /WX         # Treat linker warnings as errors
    /MP         # Build with multiple processes
    /wd4996     # Disable deprecation warnings such as fopen -> fopen_s
    /wd4458     # Disable warnings of argument names matching class members
    )
set_property(GLOBAL APPEND PROPERTY LINK_FLAGS /Debug)

set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo Release CACHE STRING "Configuration" FORCE)
set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty CACHE PATH "ThirdParty Directory")
set(QT6_CMAKE_DIR "C:/Qt/6.5.1/msvc2019_64/lib/cmake/" CACHE PATH "Qt6 CMake Directory")

# Add location of Find[package].cmake files for finding third party packages
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${THIRDPARTY_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)   # Executables and shared libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)   # Loadable module
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)   # Static libraries
set(generated_dir ${CMAKE_CURRENT_BINARY_DIR}/generated)

# Generate projects
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Editor)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Outpost731)

# Default Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LimbicEditor)