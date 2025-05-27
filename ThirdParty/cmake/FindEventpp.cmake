# Set up as imported shared library
add_library(Eventpp INTERFACE)

# Set locations for headers
target_include_directories(Eventpp
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../Eventpp-0.1.3/include"
    )

# For future reference: IMPORTED_LOCATION_<CONFIG> should be used if
# Release/Debug binaries are added.

# Refer to imported library using common cmake format: <package>::<component>
add_library(Eventpp::Eventpp ALIAS Eventpp)