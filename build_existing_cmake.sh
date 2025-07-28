#!/bin/bash

# AtlasTech2 Build Script using Existing CMake System
# This script builds Arena using the existing CMake infrastructure

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}AtlasTech2 Build Script (Using Existing CMake)${NC}"
echo "================================================"

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo -e "${RED}Error: CMakeLists.txt not found. Please run this script from the src directory.${NC}"
    exit 1
fi

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

echo -e "${YELLOW}Checking build requirements...${NC}"

# Check for required tools
MISSING_TOOLS=()

if ! command_exists cmake; then
    MISSING_TOOLS+=("cmake")
fi

if ! command_exists make; then
    MISSING_TOOLS+=("make")
fi

if ! command_exists g++; then
    MISSING_TOOLS+=("g++")
fi

if ! command_exists pkg-config; then
    MISSING_TOOLS+=("pkg-config")
fi

if [ ${#MISSING_TOOLS[@]} -ne 0 ]; then
    echo -e "${RED}Missing required tools:${NC}"
    for tool in "${MISSING_TOOLS[@]}"; do
        echo "  - $tool"
    done
    echo ""
    echo -e "${YELLOW}Install missing tools with:${NC}"
    echo "Ubuntu/Debian: sudo apt install build-essential cmake pkg-config"
    echo "Fedora: sudo dnf install gcc-c++ cmake pkg-config make"
    echo "Arch: sudo pacman -S base-devel cmake pkg-config"
    exit 1
fi

echo -e "${GREEN}✓${NC} Build tools found"

# Check for SDL2 (required for minimal Arena)
echo -e "${YELLOW}Checking for SDL2...${NC}"
if pkg-config --exists sdl2 2>/dev/null; then
    SDL2_VERSION=$(pkg-config --modversion sdl2)
    echo -e "${GREEN}✓${NC} SDL2 $SDL2_VERSION found"
else
    echo -e "${RED}✗${NC} SDL2 not found"
    echo ""
    echo -e "${YELLOW}Install SDL2 with:${NC}"
    echo "Ubuntu/Debian: sudo apt install libsdl2-dev"
    echo "Fedora: sudo dnf install SDL2-devel"
    echo "Arch: sudo pacman -S sdl2"
    echo ""
    read -p "Continue anyway? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Check for OpenGL
echo -e "${YELLOW}Checking for OpenGL...${NC}"
if pkg-config --exists gl 2>/dev/null; then
    echo -e "${GREEN}✓${NC} OpenGL found"
elif ldconfig -p | grep -q "libGL.so"; then
    echo -e "${GREEN}✓${NC} OpenGL found (via ldconfig)"
else
    echo -e "${YELLOW}Warning:${NC} OpenGL might not be available"
    echo "You may need to install: sudo apt install libgl1-mesa-dev"
fi

# Set build type
BUILD_TYPE=${1:-Debug}

if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
    echo -e "${RED}Error: Build type must be 'Debug' or 'Release'${NC}"
    echo "Usage: $0 [Debug|Release]"
    exit 1
fi

echo -e "${YELLOW}Building in $BUILD_TYPE mode...${NC}"

# Create build directory
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Cleaning existing build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake using existing system
echo -e "${YELLOW}Configuring with CMake (existing system)...${NC}"
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DBUILD=$BUILD_TYPE \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      ..

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed${NC}"
    echo ""
    echo -e "${YELLOW}Common issues:${NC}"
    echo "1. Missing dependencies (SDL2, OpenGL)"
    echo "2. User environment file missing (~/.dev/atlastech2)"
    echo "3. Complex engine dependencies"
    echo ""
    echo -e "${YELLOW}Try creating a minimal user environment file:${NC}"
    echo "mkdir -p ~/.dev && touch ~/.dev/atlastech2"
    exit 1
fi

# Build the project
echo -e "${YELLOW}Building Arena...${NC}"
make Arena -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Arena build failed${NC}"
    echo ""
    echo -e "${YELLOW}Troubleshooting tips:${NC}"
    echo "1. Check that ArenaMinimal.cpp exists in Projects/Arena/"
    echo "2. Verify SDL2 and OpenGL are properly installed"
    echo "3. Check CMake configuration output above for errors"
    echo ""
    echo -e "${YELLOW}If engine dependencies are causing issues, they have been disabled.${NC}"
    echo "The minimal Arena should build without the full engine."
    exit 1
fi

echo -e "${GREEN}Build completed successfully!${NC}"
echo ""
echo -e "${BLUE}Build artifacts:${NC}"
echo "  Executable: $(pwd)/bin/Arena"
echo "  Build directory: $(pwd)"
echo ""

# Check if Arena executable was created
if [ -f "bin/Arena" ]; then
    echo -e "${GREEN}✓${NC} Arena executable created successfully!"
    echo ""
    echo -e "${YELLOW}To run Arena:${NC}"
    echo "  cd $(pwd)/bin && ./Arena"
    echo ""
    echo -e "${BLUE}Arena Controls:${NC}"
    echo "  WASD      - Move camera"
    echo "  Space     - Move up"
    echo "  Shift     - Move down"
    echo "  Mouse     - Look around"
    echo "  F1        - Toggle mouse capture"
    echo "  ESC       - Exit"
    echo ""

    # Optional: Run the application
    read -p "Would you like to run Arena now? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        cd bin
        echo -e "${GREEN}Starting Arena...${NC}"
        echo -e "${YELLOW}Press ESC to exit${NC}"
        ./Arena
    fi
else
    echo -e "${RED}✗${NC} Arena executable not found in bin/"
    echo "Build may have succeeded but executable was not created properly."
    echo ""
    echo -e "${YELLOW}Check the build output above for clues.${NC}"
    exit 1
fi

echo ""
echo -e "${BLUE}Build Summary:${NC}"
echo "  Build type: $BUILD_TYPE"
echo "  CMake system: Existing AtlasTech2 infrastructure"
echo "  Build directory: $(pwd)"
echo "  SDL2 version: $(pkg-config --modversion sdl2 2>/dev/null || echo 'Unknown')"
echo "  Arena version: Minimal SDL implementation"
echo ""
echo -e "${GREEN}AtlasTech2 Arena build completed successfully!${NC}"
