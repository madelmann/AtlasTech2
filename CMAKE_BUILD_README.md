# AtlasTech2 CMake Build System

This document describes how to build the AtlasTech2 game engine using the modern CMake build system.

## Overview

The AtlasTech2 project has been converted from Visual Studio 2019 to use CMake for cross-platform building. The build system supports both Windows (Visual Studio/MinGW) and Unix-like systems (Linux, macOS).

## Prerequisites

### Required
- CMake 3.16 or higher
- C++14 compatible compiler:
  - Windows: Visual Studio 2019 or later, MinGW-w64
  - Linux: GCC 7+ or Clang 6+
  - macOS: Xcode 10+ or Clang 6+
- OpenGL development libraries

### Optional Dependencies
- **Boost**: For enhanced functionality (system, filesystem, thread components)
- **CURL**: For networking features
- **JSON library**: nlohmann/json or custom JSON implementation
- **MySQL**: For database functionality
- **SDL2**: For enhanced input/audio support
- **OpenAL**: For audio on Unix-like systems

## Quick Start

### Windows (Visual Studio)

```bash
# Clone and navigate to the project
cd src

# Create build directory
mkdir build
cd build

# Generate Visual Studio project files
cmake .. -G "Visual Studio 16 2019" -A Win32

# Build the project
cmake --build . --config Debug
# or
cmake --build . --config Release
```

### Windows (MinGW)

```bash
cd src
mkdir build
cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
make -j4
```

### Linux/macOS

```bash
cd src
mkdir build
cd build

# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

# Release build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

## Project Structure

After building, the following structure will be created:

```
build/
├── bin/                    # Executable files
│   ├── Debug/             # Debug executables (Windows)
│   ├── Release/           # Release executables (Windows)
│   ├── TestDrive          # TestDrive application
│   ├── WorldEditor        # WorldEditor application
│   ├── SpaceInvaders      # SpaceInvaders game
│   └── ShaderCompiler     # Shader compilation tool
└── lib/                   # Library files
    ├── Debug/             # Debug libraries (Windows)
    ├── Release/           # Release libraries (Windows)
    └── libAtlasEngine.a   # Main engine library
```

## Build Targets

The project builds the following targets:

### Libraries
- **AtlasEngine**: Main game engine static library

### Applications
- **TestDrive**: Vehicle simulation demo
- **WorldEditor**: 3D world editing tool
- **SpaceInvaders**: Classic arcade game implementation
- **ISCViewer**: ISC file viewer (if available)
- **Arena**: Arena-based game (if available)

### Tools
- **ShaderCompiler**: Shader compilation utility

## Configuration Options

### Extension Options
You can enable/disable various features using CMake options:

```bash
cmake .. -DUSE_JSON_EXTENSION=ON \
         -DUSE_MYSQL_EXTENSION=OFF \
         -DUSE_AUDIO_EXTENSION=ON \
         -DUSE_PHYSICS_EXTENSION=ON \
         -DUSE_SCRIPTING_EXTENSION=ON \
         -DUSE_NETWORKING_EXTENSION=OFF
```

### Dependency Configuration
If dependencies are not found automatically, you can specify their locations:

```bash
# Using environment variables (recommended for custom installations)
export BUILD_BOOST_INC=/path/to/boost/include
export BUILD_BOOST_LIB=/path/to/boost/lib
export BUILD_CURL_INC=/path/to/curl/include
export BUILD_CURL_LIB=/path/to/curl/lib

# Or using CMake variables
cmake .. -DBUILD_BOOST_INC=/path/to/boost/include \
         -DBUILD_BOOST_LIB=/path/to/boost/lib
```

### User Environment File
Create a user environment file at `~/.dev/atlastech2.cmake` (Linux/macOS) or `%USERPROFILE%\.dev\atlastech2.cmake` (Windows):

```cmake
# Example user environment file
set(BUILD_BOOST_INC "/usr/local/include")
set(BUILD_BOOST_LIB "/usr/local/lib")
set(BUILD_CURL_INC "/usr/include/curl")
set(BUILD_CURL_LIB "/usr/lib")
set(BUILD_JSON_INC "/usr/local/include")
set(BUILD_JSON_LIB "/usr/local/lib")
```

## Package Manager Integration

### vcpkg (Windows)
```bash
# Install dependencies
vcpkg install boost curl nlohmann-json opengl

# Use vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### pkg-config (Linux)
The build system automatically detects libraries through pkg-config when available:

```bash
# Install development packages (Ubuntu/Debian)
sudo apt-get install libboost-all-dev libcurl4-openssl-dev \
                     nlohmann-json3-dev libsdl2-dev libopenal-dev \
                     libmysqlclient-dev

# Install development packages (Fedora/CentOS)
sudo dnf install boost-devel libcurl-devel json-devel \
                 SDL2-devel openal-soft-devel mysql-devel
```

## Troubleshooting

### Common Issues

1. **CMake version too old**
   ```
   Error: CMake 3.16 or higher is required
   ```
   Solution: Update CMake or use a newer version.

2. **Compiler not found**
   ```
   Error: No CMAKE_CXX_COMPILER could be found
   ```
   Solution: Install a C++ compiler and ensure it's in your PATH.

3. **OpenGL not found**
   ```
   Error: Could NOT find OpenGL
   ```
   Solution: Install OpenGL development libraries.

4. **Missing dependencies**
   ```
   Warning: Boost not found and BUILD_BOOST_INC not set
   ```
   Solution: Either install the dependency or set the path manually.

### Debug Build Issues
- Ensure all source files exist and are readable
- Check that the Visual Studio project files are up to date
- Verify that all required dependencies are available

### Platform-Specific Notes

#### Windows
- Use the same architecture (Win32/x64) for all dependencies
- Ensure Visual Studio redistributables are installed for deployment
- Some antivirus software may interfere with the build process

#### Linux
- Install development packages for all dependencies
- Ensure mesa-common-dev is installed for OpenGL headers
- Use `make VERBOSE=1` for detailed build output

#### macOS
- Install Xcode command line tools: `xcode-select --install`
- Use Homebrew for easy dependency management
- May need to set `MACOSX_DEPLOYMENT_TARGET`

## Performance and Optimization

### Release Builds
For optimal performance, always use Release builds for final applications:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Parallel Building
Use parallel compilation to speed up builds:
- Linux/macOS: `make -j$(nproc)`
- Windows: `cmake --build . -- /m`

## Development Workflow

1. Make changes to source code
2. Run `cmake --build .` to rebuild
3. Test the applications in the `bin/` directory
4. For major configuration changes, delete the build directory and reconfigure

## Contributing

When contributing to the project:
1. Test builds on multiple platforms if possible
2. Update this README if adding new dependencies or build options
3. Ensure CMakeLists.txt files follow the established patterns
4. Document any new configuration options

## Support

For build system issues:
1. Check this README for common solutions
2. Verify CMake and compiler versions
3. Test with a clean build directory
4. Check the CMake cache for configuration issues: `cmake -L`

For more detailed build information, use:
```bash
cmake --build . --verbose
```
