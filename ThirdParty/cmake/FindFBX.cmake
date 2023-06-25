# Convert "Program Files" path to forward slashes
cmake_path(CONVERT "$ENV{ProgramFiles}" TO_CMAKE_PATH_LIST programfiles_path)

# Path to FBX SDK
set(fbx_path "${programfiles_path}/Autodesk/FBX/FBX SDK/2020.0.1" CACHE PATH "FBX SDK Directory")

# Set up as imported shared library
add_library(FBX SHARED IMPORTED)

# Set locations for .lib, .dll, and headers
set_target_properties(FBX PROPERTIES
    IMPORTED_LOCATION
        "${fbx_path}/lib/vs2017/x64/release/libfbxsdk.dll"
    IMPORTED_LOCATION_DEBUG
        "${fbx_path}/lib/vs2017/x64/debug/libfbxsdk.dll"

    IMPORTED_IMPLIB
        "${fbx_path}/lib/vs2017/x64/release/libfbxsdk.lib"
    IMPORTED_IMPLIB_DEBUG
        "${fbx_path}/lib/vs2017/x64/debug/libfbxsdk.lib"

    #INTERFACE_INCLUDE_DIRECTORIES
    #    "${fbx_path}/include"

    LINKER_LANGUAGE CXX
    )

target_include_directories(FBX SYSTEM INTERFACE "${fbx_path}/include")
    
# For future reference: IMPORTED_LOCATION_<CONFIG> should be used if
# Release/Debug binaries are added.

# Refer to imported library using common cmake format: <package>::<component>
add_library(FBX::FBX ALIAS FBX)