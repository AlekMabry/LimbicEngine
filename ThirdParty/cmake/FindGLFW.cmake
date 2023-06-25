# Set up as imported shared library
add_library(GLFW SHARED IMPORTED)

# Set locations for .lib, .dll, and headers
set_target_properties(GLFW PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../GLFW-3.3.8/bin/glfw3.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_LIST_DIR}/../GLFW-3.3.8/lib/glfw3dll.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../GLFW-3.3.8/include"
    )

# For future reference: IMPORTED_LOCATION_<CONFIG> should be used if
# Release/Debug binaries are added.

# Refer to imported library using common cmake format: <package>::<component>
add_library(GLFW::GLFW ALIAS GLFW)