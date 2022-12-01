# Limbic Engine
A custom lightweight 3D VR game engine.

## Building
Limbic Engine is written in Visual Studio for Windows x64, and supports x64
Release and Debug builds. The solution is formatted:

* LimbicEngine project
    * The engine/game source.
    * Compiles to a static library.
* LimbicEditor project
    * A Qt6 game editor wrapper for the engine library.
* Outpost731 project
    * The game executable, a wrapper for the engine library.

## Dependencies
Binaries and header files for most dependencies are in `Library/`.

### Qt6
Qt6 can be installed with the [Qt installer](https://qt.io/download)).

### Vulkan
Vulkan SDK (version 1.3.231.1) can be installed with the [Vulkan SDK installer](https://vulkan.lunarg.com/sdk/home).

Some computers do not have Vulkan runtime preinstalled with their graphics driver, such as Intel integrated graphics. Install the runtime if you can not run examples in the Vulkan `C:/VulkanSDK/1.3.231.1/Bin` directory.