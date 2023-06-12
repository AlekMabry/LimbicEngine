cmake_minimum_required(VERSION 3.26.0)

# Project and metadata
project(
    LimbicEngine
    VERSION 0.1.0
    DESCRIPTION "Lightweight VR Game Engine"
    LANGUAGES CXX
    )

# Accumulate project files and manually place into IDE folders
set(sources
    Source/Game.cpp
    )
set(system_sources
    Source/System/IOSystem_GLFW.cpp
    Source/System/RenderSystem.cpp
    Source/System/ResourceSystem.cpp
    Source/System/WorldSystem.cpp 
    )
set(entity_sources
    Source/Entity/EEntity.cpp
    Source/Entity/EPlayer.cpp
    Source/Entity/EStaticWorldGeometry.cpp
    Source/Entity/ETestCamera.cpp
    )
set(utility_sources
    Source/Utility/VkUtil.cpp
    )
set(includes
    Include/Game.h
    Include/LimbicProperties.h
    Include/LimbicTypes.h
    )
set(system_includes
    Include/System/IOSystem.h
    Include/System/IOSystem_GLFW.h
    Include/System/RenderSystem.h
    Include/System/ResourceSystem.h
    Include/System/WorldSystem.h
)
set(entity_includes
    Include/Entity/EEntity.h
    Include/Entity/EPlayer.h
    Include/Entity/EStaticWorldGeometry.h
    Include/Entity/ETestCamera.h
)
set(utility_includes
    Include/Utility/VkUtil.h
    )

# Folders in IDE
# Todo: Use TREE
source_group("Source" FILES ${sources})
source_group("Source/System" FILES ${system_sources})
source_group("Source/Entity" FILES ${entity_sources})
source_group("Source/Utility" FILES ${utility_sources})
source_group("Include" FILES ${includes})
source_group("Include/System" FILES ${system_includes})
source_group("Include/Entity" FILES ${entity_includes})
source_group("Include/Utility" FILES ${utility_includes})

# Setup static engine library that is used as the base for the editor and game
add_library(LimbicEngine SHARED
    ${sources} ${system_sources} ${entity_sources} ${utility_sources}
    ${includes} ${system_includes} ${entity_includes} ${utility_includes}
    )
target_include_directories(LimbicEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    )
add_library(Limbic::Engine ALIAS LimbicEngine)

# Publicly linked third party libs (available to consumers of this library as well)
find_package(Vulkan REQUIRED)
find_package(GLM REQUIRED)
target_link_libraries(LimbicEngine PUBLIC
    Vulkan::Vulkan GLM::GLM)

# Privately linked third party libs
find_package(GLFW REQUIRED)
find_package(FBX REQUIRED)
find_package(RapidJSON REQUIRED)
target_link_libraries(LimbicEngine PRIVATE
    GLFW::GLFW FBX::FBX RapidJSON::RapidJSON)