# Limbic Engine
A custom lightweight 3D VR game engine.

## Building
Limbic Engine is written in Visual Studio for Windows x64, and supports x64 Release and Debug
builds.

To generate the visual studio solution with cmake:

    mkdir build
    cd build
    cmake -G"Visual Studio 17 2022" -A x64 ..

## Dependencies
Binaries and header files for most dependencies are in `ThirdParty/`.

### Qt6
Qt6 can be installed with the [Qt installer](https://qt.io/download)).

### Vulkan
The Vulkan SDK can be installed with the [Vulkan SDK installer](https://vulkan.lunarg.com/sdk/home).

Some computers do not have Vulkan runtime preinstalled with their graphics driver, such as Intel
integrated graphics. Install the runtime if you can not run examples in the Vulkan `C:/VulkanSDK/<Version>/Bin` directory.