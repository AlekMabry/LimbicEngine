# Set up as imported shared library
add_library(RapidJSON INTERFACE)

# Set locations for headers
target_include_directories(RapidJSON
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../RapidJSON-1.1.0/include"
    )

# For future reference: IMPORTED_LOCATION_<CONFIG> should be used if
# Release/Debug binaries are added.

# Refer to imported library using common cmake format: <package>::<component>
add_library(RapidJSON::RapidJSON ALIAS RapidJSON)