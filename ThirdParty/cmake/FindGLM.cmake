# Set up as imported shared library
add_library(GLM INTERFACE)

# Set locations for headers
target_include_directories(GLM
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../GLM-0.9.9/include"
    )

# For future reference: IMPORTED_LOCATION_<CONFIG> should be used if
# Release/Debug binaries are added.

# Refer to imported library using common cmake format: <package>::<component>
add_library(GLM::GLM ALIAS GLM)