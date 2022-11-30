# Limbic Engine
A custom lightweight 3D VR game engine.

# Building
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
Binaries and header files for dependencies are in `Library/` (With the exception
of Qt6, which must be installed with their ![installer](https://qt.io/download)).