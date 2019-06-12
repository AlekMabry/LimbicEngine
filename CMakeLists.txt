cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(LimbicEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_HOME_DIRECTORY}/build)

add_compile_definitions()

set(OpenGL_GL_PREFERENCE GLVND)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
#include_directories(${)

# Add source files
file(GLOB SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/LimbicEngine/*.c
        ${CMAKE_SOURCE_DIR}/LimbicEngine/*.cpp)

file(GLOB HEADER_FILES
        ${CMAKE_SOURCE_DIR}/LimbicEngine/*.h
        ${CMAKE_SOURCE_DIR}/LimbicEngine/*.hpp)

add_executable(LimbicEngine ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(LimbicEngine ${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES}
        ${GLEW_LIBRARIES} glm)