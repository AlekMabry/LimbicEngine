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
    Source/main.cpp
    Source/EditorWindow.cpp
    Source/SettingsDialog.cpp
    Source/MapTreeModel.cpp
    Source/NodePropertiesWidget.cpp
    Source/ViewWidget.cpp
    )
set(includes
    Include/EditorTypes.h
    Include/EditorWindow.h
    Include/SettingsDialog.h
    Include/MapTreeModel.h
    Include/NodePropertiesWidget.h
    Include/ViewWidget.h
    )
set(qt_files
    Qt/Editor.ui
    Qt/Resources.ui
    Qt/Settings.ui
    Qt/Widget.ui
    Qt/NodePropertiesWidget.ui
    )
set(qt_rc
    Qt/LimbicEditor.qrc
    )

# Folders in IDE
source_group("Source" FILES ${sources})
source_group("Include" FILES ${includes})
source_group("Qt" FILES ${qt_files} ${qt_rc})

# Setup executable
add_executable(LimbicEditor WIN32 ${sources} ${includes} ${qt_files})

# Link header files
target_include_directories(LimbicEditor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Include")

# Link engine shared library
target_link_libraries(LimbicEditor PRIVATE Limbic::Engine)

# Link Qt
set(CMAKE_PREFIX_PATH ${QT6_CMAKE_DIR})
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
qt_standard_project_setup()
qt6_add_resources(resource_files ${qt_rc})

qt_wrap_ui(UI_HEADERS ${qt_files})   # Rule for calling uic on .ui files. For each input, a header is generate
target_link_libraries(LimbicEditor PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
target_sources(LimbicEditor PRIVATE ${resource_files})

# Use windeployqt.exe to copy the required runtime libraries
get_target_property(qmake_exe Qt6::qmake IMPORTED_LOCATION)
get_filename_component(qmake_dir "${qmake_exe}" DIRECTORY)
#string(TOLOWER $<CONFIG> config_lower)
add_custom_command(TARGET LimbicEditor POST_BUILD
    COMMAND "${qmake_dir}/windeployqt.exe"
        --verbose 1
        $<$<CONFIG:Debug>:--debug>
        $<$<CONFIG:Release>:--release>
        \"$<TARGET_FILE:LimbicEditor>\"
    )

# add_custom_command(TARGET LimbicEditor POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Core> "${CMAKE_BINARY_DIR}/$<CONFIG>"
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Gui> "${CMAKE_BINARY_DIR}/$<CONFIG>"
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Widgets> "${CMAKE_BINARY_DIR}/$<CONFIG>"
#     )