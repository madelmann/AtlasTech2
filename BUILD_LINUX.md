# Building AtlasTech2 on Linux

This guide will help you convert and build the AtlasTech2 OpenGL application from Windows to Linux using SDL2.

## Prerequisites

### System Requirements
- Linux distribution (Ubuntu 18.04+, Fedora 30+, or equivalent)
- GCC 7+ with C++14 support
- CMake 3.5 or higher
- Git (for version control)

### Required Libraries

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake pkg-config git
sudo apt install libsdl2-dev libsdl2-image-dev
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
sudo apt install libopenal-dev libsndfile1-dev
sudo apt install libboost-all-dev  # If using Boost libraries
```

#### Fedora/RHEL/CentOS
```bash
sudo dnf install gcc-c++ cmake pkg-config git
sudo dnf install SDL2-devel SDL2_image-devel
sudo dnf install mesa-libGL-devel mesa-libGLU-devel
sudo dnf install openal-soft-devel libsndfile-devel
sudo dnf install boost-devel  # If using Boost libraries
```

#### Arch Linux
```bash
sudo pacman -S base-devel cmake pkg-config git
sudo pacman -S sdl2 sdl2_image
sudo pacman -S mesa glu
sudo pacman -S openal libsndfile
sudo pacman -S boost  # If using Boost libraries
```

## Quick Start

### Automated Build
The easiest way to build the project is using the provided build script:

```bash
cd src
./build_linux.sh Debug
```

Or for a release build:
```bash
./build_linux.sh Release
```

### Manual Build
If you prefer to build manually:

```bash
cd src
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD=Debug ..
make -j$(nproc)
```

## Architecture Overview

### Conversion Strategy
The Windows-to-Linux conversion includes:

1. **Entry Point**: Converted `WinMain()` to standard `main()`
2. **Window Management**: Replaced Windows API with SDL2 window creation
3. **Event Handling**: Converted Windows messages to SDL events
4. **Mouse Input**: Replaced Windows mouse functions with SDL equivalents
5. **OpenGL Context**: Used SDL2 for cross-platform OpenGL context creation
6. **Build System**: CMake configuration for Linux dependencies

### Key Components

#### SDL Context (`src/Projects/Engine/Driver/SDL/Context.cpp`)
- Cross-platform OpenGL context creation
- Window management and resize handling
- VSync support

#### Application Framework (`src/Projects/Engine/Application/Application.cpp`)
- SDL event loop instead of Windows message pump
- Cross-platform input handling
- Frame rate management

#### Compatibility Layer (`src/Projects/Engine/Common/SDLCompat.h`)
- Windows API compatibility definitions
- Virtual key code mappings
- Helper functions for smooth transition

## Project Structure

```
src/
├── Projects/
│   ├── Engine/           # Core engine library
│   │   ├── Driver/SDL/   # SDL implementation
│   │   ├── Application/  # Cross-platform app framework
│   │   └── Common/       # Compatibility layer
│   ├── Arena/            # Main application
│   └── ObjectiveScript/  # Scripting system
├── CMake/                # Build configuration
├── build/                # Build output (generated)
├── BUILD_LINUX.md        # This file
└── build_linux.sh        # Build script
```

## Detailed Build Process

### 1. Dependency Installation
Ensure all required libraries are installed. The build script will check for these automatically.

### 2. Configuration
CMake will configure the build based on your system:
- Detect SDL2 and related libraries
- Set up OpenGL linking
- Configure compiler flags for C++14

### 3. Compilation
The build process creates:
- `AtlasEngine` - Core engine library
- `ObjectiveScript` - Scripting system library  
- `Arena` - Main executable

### 4. Output
Built files are placed in:
- `build/bin/` - Executables
- `build/lib/` - Libraries

## Running the Application

After building, run the application:

```bash
cd build/bin
./Arena
```

### Configuration Files
The application expects these configuration files in the same directory:
- `engine.conf` - Engine settings
- `tcpclient.conf` - Network client configuration
- `tcpserver.conf` - Network server configuration
- `base/` - Directory with shaders and assets

## Troubleshooting

### Common Build Issues

#### SDL2 Not Found
```
CMake Error: Could not find SDL2
```
**Solution**: Install SDL2 development packages:
```bash
sudo apt install libsdl2-dev libsdl2-image-dev
```

#### OpenGL Linking Errors
```
undefined reference to `glGenBuffers`
```
**Solution**: Install Mesa development packages:
```bash
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
```

#### C++14 Compilation Errors
```
error: 'auto' not allowed in non-static class member
```
**Solution**: Ensure you have GCC 7+ or Clang 5+:
```bash
gcc --version
# Should show 7.0 or higher
```

### Runtime Issues

#### Missing Libraries
```
error while loading shared libraries: libSDL2-2.0.so.0
```
**Solution**: Install runtime libraries:
```bash
sudo apt install libsdl2-2.0-0 libsdl2-image-2.0-0
```

#### OpenGL Context Creation Failed
**Solution**: Ensure you have proper graphics drivers installed:
```bash
# For NVIDIA
sudo apt install nvidia-driver-470

# For AMD
sudo apt install mesa-vulkan-drivers

# For Intel
sudo apt install intel-media-va-driver
```

#### Audio Issues
```
Could not initialize OpenAL
```
**Solution**: Install OpenAL and check audio system:
```bash
sudo apt install libopenal1 pulseaudio
pulseaudio --check
```

## Development

### Adding New Features
When adding new Windows-specific code, use the compatibility layer:

```cpp
#include <Common/SDLCompat.h>

// This works on both Windows and Linux
POINT mousePos;
GetCursorPos(&mousePos);
```

### Debugging
Build with debug symbols:
```bash
./build_linux.sh Debug
```

Use GDB for debugging:
```bash
cd build/bin
gdb ./Arena
```

### Code Style
The project uses:
- C++14 standard
- CMake for build configuration
- GCC/Clang compiler warnings enabled
- Pedantic compilation flags

## Performance Notes

### Optimization
For release builds, the following optimizations are applied:
- `-O4` optimization level
- Link-time optimization where supported
- Debug symbols stripped

### Memory Management
- No Windows-specific memory debugging on Linux
- Use Valgrind for memory leak detection:
```bash
cd build/bin
valgrind --leak-check=full ./Arena
```

## Contributing

### Code Conversion Guidelines
When converting Windows-specific code:

1. Use SDL2 equivalents where possible
2. Add compatibility definitions to `SDLCompat.h`
3. Test on multiple Linux distributions
4. Maintain Windows compatibility where feasible

### Testing
Test your changes on:
- Ubuntu LTS (latest)
- Fedora (latest)
- Different graphics drivers (NVIDIA, AMD, Intel)

## Known Limitations

1. **Font Rendering**: Some Windows-specific font handling may need adjustment
2. **File Paths**: Windows backslashes converted to forward slashes
3. **Registry Access**: Not available on Linux (use config files instead)
4. **Windows-specific APIs**: Some functionality may have different behavior

## Support

For build issues or questions:
1. Check this documentation first
2. Review the build script output for specific error messages
3. Ensure all dependencies are installed correctly
4. Test with a minimal build to isolate issues

The conversion maintains the original application architecture while providing cross-platform compatibility through SDL2 and careful abstraction of platform-specific functionality.