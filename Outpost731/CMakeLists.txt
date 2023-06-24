cmake_minimum_required(VERSION 3.26.0)

# Project and metadata
project(
    Outpost731
    VERSION 0.1.0
    DESCRIPTION "VR Game"
    LANGUAGES CXX
    )

# Accumulate project files
set(sources
    Source/Outpost731.cpp
)

# Folders in IDE
source_group("Source" FILES ${sources})

# Setup executable
add_executable(Outpost731 ${sources})

# Link engine shared library
target_link_libraries(Outpost731 PUBLIC Limbic::Engine)