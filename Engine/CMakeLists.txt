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
    Source/System/EventSystem.cpp
    )
set(entity_sources
    Source/Entity/EEntity.cpp
    Source/Entity/EPlayer.cpp
    Source/Entity/EStaticWorldGeometry.cpp
    Source/Entity/ETestCamera.cpp
    )
set(utility_sources
    )
set(renderer_sources
    Source/Renderer/RView.cpp
    Source/Renderer/RWindow.cpp
    Source/Renderer/RWindow_GLFW.cpp
    )
set(includes
    Include/Game.h
    Include/LimbicProperties.h
    Include/LimbicTypes.h
    )
set(system_includes
    Include/System/IOSystem.h
    Include/System/IOSystem_GLFW.h
    Include/System/IOSystem_GLFWBinds.h
    Include/System/IOSystem_InputBinds.h
    Include/System/RenderSystem.h
    Include/System/ResourceSystem.h
    Include/System/WorldSystem.h
    Include/System/EventSystem.h
)
set(entity_includes
    Include/Entity/EEntity.h
    Include/Entity/EPlayer.h
    Include/Entity/EStaticWorldGeometry.h
    Include/Entity/ETestCamera.h
)
set(utility_includes
    )
set(renderer_includes
    Include/Renderer/RView.h
    Include/Renderer/RWindow.h
    Include/Renderer/RWindow_GLFW.h
    )
set(shaders
    Shader/hardcodedTriangle.vert
    Shader/hardcodedTriangle.frag
    Shader/passthrough.vert
    Shader/passthrough.frag
    Shader/postprocess.vert
    Shader/postprocess.frag
    Shader/spinning.vert
    Shader/spinning.frag
    Shader/staticPBR.vert
    Shader/staticPBR.frag
    Shader/texmapped.vert
    Shader/texmapped.frag
)

# Folders in IDE
# Todo: Use TREE
source_group("Source" FILES ${sources})
source_group("Source/System" FILES ${system_sources})
source_group("Source/Entity" FILES ${entity_sources})
source_group("Source/Utility" FILES ${utility_sources})
source_group("Source/Renderer" FILES ${renderer_sources})
source_group("Include" FILES ${includes})
source_group("Include/System" FILES ${system_includes})
source_group("Include/Entity" FILES ${entity_includes})
source_group("Include/Utility" FILES ${utility_includes})
source_group("Include/Renderer" FILES ${renderer_includes})
source_group("Shader" FILES ${shaders})

# Setup static engine library that is used as the base for the editor and game
add_library(LimbicEngine STATIC
    ${sources} ${system_sources} ${entity_sources} ${utility_sources} ${renderer_sources}
    ${includes} ${system_includes} ${entity_includes} ${utility_includes} ${renderer_includes}
    )
target_include_directories(LimbicEngine
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include"
    )
add_library(Limbic::Engine ALIAS LimbicEngine)

# Publicly linked third party libs (available to consumers of this library as well)
find_package(Vulkan REQUIRED COMPONENTS glslc)
find_package(GLM REQUIRED)
find_package(GLFW REQUIRED)     # Todo: This should not be public, IOSystem_GLFW.h needs GLFW code isolated
find_package(Eventpp REQUIRED)
target_link_libraries(LimbicEngine PUBLIC
    Vulkan::Vulkan GLM::GLM GLFW::GLFW Eventpp::Eventpp)

# Privately linked third party libs
find_package(FBX REQUIRED)
find_package(RapidJSON REQUIRED)
target_link_libraries(LimbicEngine PUBLIC
    FBX::FBX RapidJSON::RapidJSON)

# Copy runtime .dlls of linked libraries
add_custom_command(TARGET LimbicEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:GLFW::GLFW> ${CMAKE_BINARY_DIR}/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:FBX::FBX> ${CMAKE_BINARY_DIR}/$<CONFIG>
    )

# Create shaders folder
add_custom_command(TARGET LimbicEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIG>/Shader
    )

# Build shaders
foreach(shader ${shaders})
    get_filename_component(shader_name ${shader} NAME_WE)

    # Convert <shader>.type to <shader><Type>.spv
    get_filename_component(shader_ext  ${shader} LAST_EXT)
    string(REPLACE .vert Vert shader_ext ${shader_ext})
    string(REPLACE .frag Frag shader_ext ${shader_ext})

    add_custom_command(TARGET LimbicEngine POST_BUILD
      COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${shader}
        -o ${CMAKE_BINARY_DIR}/$<CONFIG>/Shader/${shader_name}${shader_ext}.spv
      )
endforeach()