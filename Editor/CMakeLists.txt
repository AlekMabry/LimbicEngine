cmake_minimum_required(VERSION 3.26.0)

# Project and metadata
project(
    LimbicEditor
    VERSION 0.1.0
    DESCRIPTION "Editor for Limbic Engine"
    LANGUAGES CXX
    )

# Accumulate project files
set(sources
    Source/EditorWindow.cpp
    Source/LimbicEditor.cpp
    Source/SettingsDialog.cpp
    Source/ViewPortWidget.cpp
    )
set(includes
    Include/EditorTypes.h
    Include/EditorWindow.h
    Include/SettingsDialog.h
    Include/ViewPortWidget.h
    )
set(uis
    UI/Editor.ui
    UI/Resources.ui
    UI/Settings.ui
    UI/Widget.ui
    )
set(resources
    UI/LimbicEditor.qrc
    )

# Folders in IDE
source_group("Source" FILES ${sources})
source_group("Include" FILES ${includes})
source_group("UI" FILES ${uis})
source_group("Resource" FILES ${resources})

# Setup executable
add_executable(LimbicEditor WIN32 ${sources} ${includes} ${uis} ${resources})

# Link engine shared library
target_link_libraries(LimbicEditor PRIVATE Limbic::Engine)
target_include_directories(
    LimbicEditor PRIVATE $<TARGET_PROPERTY:Limbic::Engine,INTERFACE_INCLUDE_DIRECTORIES>
    )

# Link Qt
set(CMAKE_PREFIX_PATH ${QT6_CMAKE_DIR})
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()
qt_wrap_ui(UI_HEADERS ${uis})   # Rule for calling uic on .ui files. For each input, a header is generate
target_link_libraries(LimbicEditor PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)