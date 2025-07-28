#!/bin/bash

# AtlasTech2 Minimal Arena Build Script
# Automated build script for the working SDL/OpenGL Arena application

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}AtlasTech2 Minimal Arena Build Script${NC}"
echo "======================================="

# Check if we're in the right directory
if [ ! -f "arena_minimal.cpp" ]; then
    echo -e "${RED}Error: arena_minimal.cpp not found. Please run this script from the src directory.${NC}"
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

# Check for SDL2
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
BUILD_TYPE=${1:-Release}

if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
    echo -e "${RED}Error: Build type must be 'Debug' or 'Release'${NC}"
    echo "Usage: $0 [Debug|Release]"
    exit 1
fi

echo -e "${YELLOW}Building in $BUILD_TYPE mode...${NC}"

# Create build directory
BUILD_DIR="build_arena"
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Cleaning existing build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Copy source files
echo -e "${YELLOW}Preparing build files...${NC}"
cp ../CMakeLists_arena_minimal.txt ./CMakeLists.txt
cp ../arena_minimal.cpp .

# Configure with CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE .

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed${NC}"
    exit 1
fi

# Build the project
echo -e "${YELLOW}Building Arena...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed${NC}"
    exit 1
fi

echo -e "${GREEN}Build completed successfully!${NC}"
echo ""
echo -e "${BLUE}Build artifacts:${NC}"
echo "  Executable: $(pwd)/bin/arena_minimal"
echo ""

# Test if executable was created
if [ -f "bin/arena_minimal" ]; then
    echo -e "${GREEN}✓${NC} Arena executable created successfully!"
    echo ""
    echo -e "${YELLOW}To run Arena:${NC}"
    echo "  cd $(pwd)/bin && ./arena_minimal"
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
        ./arena_minimal
    fi
else
    echo -e "${RED}✗${NC} Arena executable not found in bin/"
    echo "Build may have succeeded but executable was not created properly."
    exit 1
fi

echo ""
echo -e "${BLUE}Build Summary:${NC}"
echo "  Build type: $BUILD_TYPE"
echo "  Build directory: $(pwd)"
echo "  SDL2 version: $(pkg-config --modversion sdl2 2>/dev/null || echo 'Unknown')"
echo "  OpenGL: Available"
echo ""
echo -e "${GREEN}Arena build completed successfully!${NC}"
