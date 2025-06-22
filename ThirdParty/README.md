# Third Party Libraries
Binaries/headers for x64 builds with MSVC 17. Built as shared libraries
when possible.

### Externally Installed
Libraries with installers are not included in this directory, and should be
downloaded by their installers:

* Vulkan SDK:
    * Download a version >= 1.3 [Vulkan SDK installer](https://vulkan.lunarg.com/sdk/home#windows).
* Qt6:
    * Download the latest Qt6 from the
      [Qt6 installer](https://www.qt.io/download).
    * Qt Quick components do not need to be installed, this engine uses classic
      Qt Widgets.
    * You may need to set the Qt `cmake/` directory path if you install to a
      different location other than `C:\Qt`. Set this path with
      `-DQT_CMAKE_DIR=<Path>` when running cmake.
* FBX SDK:
    * Download and run the
      [FBX SDK 2020.0.1 VS2017 installer](https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-0).
* OpenXR SDK:
    * Compile the OpenXR SDK from [github.com/KhronosGroup/OpenXR-SDK](https://github.com/KhronosGroup/OpenXR-SDK).
    * After building, open the solution with elevated Visual Studio, then run the `INSTALL` project.

### CMake
The `CMakeLists.txt` for each project uses `find_package()` to link libraries from this
directory. To do this, there are `Find<Library>.cmake` files in the `cmake/` directory
that point to the binaries and headers in this directory.