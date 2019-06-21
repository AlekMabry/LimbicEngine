cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(LimbicEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_HOME_DIRECTORY}/bin)

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

# Add source files
file(GLOB ENGINE_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/LimbicEngine/*.c
        ${CMAKE_SOURCE_DIR}/src/LimbicEngine/*.cpp)

file(GLOB ENGINE_HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/LimbicEngine/*.h
        ${CMAKE_SOURCE_DIR}/src/LimbicEngine/*.hpp)

add_executable(LimbicEngine ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})

target_link_libraries(LimbicEngine ${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES}
        ${GLEW_LIBRARIES} glm)

# Now compile the editor
#-----------------------------------------------------------------------------------------------------------------------
project(LimbicEditor)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")


find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

# Add source files
file(GLOB EDITOR_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/LimbicEditor/*.c
        ${CMAKE_SOURCE_DIR}/src/LimbicEditor/*.cpp)

file(GLOB EDITOR_HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/LimbicEditor/*.h
        ${CMAKE_SOURCE_DIR}/src/LimbicEditor/*.hpp)

QT5_WRAP_UI(UI_HEADERS /src/LimbicEditor/*MainWindow.ui)

add_executable(LimbicEditor ${EDITOR_SOURCE_FILES} ${EDITOR_HEADER_FILES})

set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
target_link_libraries(LimbicEditor ${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES}
        ${GLEW_LIBRARIES} Qt5::Widgets Qt5::Core Qt5::Gui glm)