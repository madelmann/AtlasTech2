// Minimal Arena implementation using SDL
// This version bypasses complex engine subsystems to get basic functionality working

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class MinimalArena {
private:
    SDL_Window* mWindow;
    SDL_GLContext mGLContext;
    bool mRunning;
    int mWidth;
    int mHeight;
    
    // Basic camera state
    float mCameraX, mCameraY, mCameraZ;
    float mCameraRotX, mCameraRotY;
    
    // Mouse state
    int mLastMouseX, mLastMouseY;
    bool mMouseCaptured;
    
public:
    MinimalArena() 
        : mWindow(nullptr)
        , mGLContext(nullptr)
        , mRunning(false)
        , mWidth(1024)
        , mHeight(768)
        , mCameraX(0.0f), mCameraY(0.0f), mCameraZ(5.0f)
        , mCameraRotX(0.0f), mCameraRotY(0.0f)
        , mLastMouseX(0), mLastMouseY(0)
        , mMouseCaptured(false)
    {
    }
    
    ~MinimalArena() {
        shutdown();
    }
    
    bool initialize() {
        std::cout << "Initializing Minimal Arena..." << std::endl;
        
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
            std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        // Set OpenGL attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        
        // Create window
        mWindow = SDL_CreateWindow(
            "AtlasTech2 Arena - Minimal",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            mWidth,
            mHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );
        
        if (!mWindow) {
            std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        // Create OpenGL context
        mGLContext = SDL_GL_CreateContext(mWindow);
        if (!mGLContext) {
            std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        // Enable VSync
        SDL_GL_SetSwapInterval(1);
        
        // Initialize OpenGL state
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        
        // Capture mouse
        SDL_SetRelativeMouseMode(SDL_TRUE);
        mMouseCaptured = true;
        
        std::cout << "Arena initialized successfully" << std::endl;
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        
        return true;
    }
    
    void shutdown() {
        if (mGLContext) {
            SDL_GL_DeleteContext(mGLContext);
            mGLContext = nullptr;
        }
        
        if (mWindow) {
            SDL_DestroyWindow(mWindow);
            mWindow = nullptr;
        }
        
        SDL_Quit();
    }
    
    void handleEvent(const SDL_Event& event) {
        switch (event.type) {
            case SDL_QUIT:
                mRunning = false;
                break;
                
            case SDL_KEYDOWN:
                handleKeyDown(event.key.keysym.sym);
                break;
                
            case SDL_MOUSEMOTION:
                if (mMouseCaptured) {
                    handleMouseMotion(event.motion.xrel, event.motion.yrel);
                }
                break;
                
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    mWidth = event.window.data1;
                    mHeight = event.window.data2;
                    glViewport(0, 0, mWidth, mHeight);
                }
                break;
        }
    }
    
    void handleKeyDown(SDL_Keycode key) {
        const float moveSpeed = 0.5f;
        
        switch (key) {
            case SDLK_ESCAPE:
                mRunning = false;
                break;
                
            case SDLK_w:
                mCameraZ -= moveSpeed;
                break;
                
            case SDLK_s:
                mCameraZ += moveSpeed;
                break;
                
            case SDLK_a:
                mCameraX -= moveSpeed;
                break;
                
            case SDLK_d:
                mCameraX += moveSpeed;
                break;
                
            case SDLK_SPACE:
                mCameraY += moveSpeed;
                break;
                
            case SDLK_LSHIFT:
                mCameraY -= moveSpeed;
                break;
                
            case SDLK_F1:
                // Toggle mouse capture
                mMouseCaptured = !mMouseCaptured;
                SDL_SetRelativeMouseMode(mMouseCaptured ? SDL_TRUE : SDL_FALSE);
                std::cout << "Mouse capture: " << (mMouseCaptured ? "ON" : "OFF") << std::endl;
                break;
        }
    }
    
    void handleMouseMotion(int xrel, int yrel) {
        const float sensitivity = 0.1f;
        
        mCameraRotY += xrel * sensitivity;
        mCameraRotX += yrel * sensitivity;
        
        // Clamp vertical rotation
        if (mCameraRotX > 90.0f) mCameraRotX = 90.0f;
        if (mCameraRotX < -90.0f) mCameraRotX = -90.0f;
    }
    
    void update() {
        // Simple update logic - could add game state updates here
    }
    
    void render() {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set up projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        // Simple perspective projection
        float aspect = (float)mWidth / (float)mHeight;
        float fov = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        
        float f = 1.0f / tan(fov * M_PI / 360.0f);
        float projMatrix[16] = {
            f/aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (farPlane+nearPlane)/(nearPlane-farPlane), 2*farPlane*nearPlane/(nearPlane-farPlane),
            0, 0, -1, 0
        };
        glLoadMatrixf(projMatrix);
        
        // Set up modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Apply camera rotation
        glRotatef(mCameraRotX, 1.0f, 0.0f, 0.0f);
        glRotatef(mCameraRotY, 0.0f, 1.0f, 0.0f);
        
        // Apply camera translation
        glTranslatef(-mCameraX, -mCameraY, -mCameraZ);
        
        // Render a simple 3D scene
        renderScene();
        
        // Draw UI overlay
        renderUI();
        
        // Swap buffers
        SDL_GL_SwapWindow(mWindow);
    }
    
    void renderScene() {
        // Draw a simple grid floor
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_LINES);
        for (int i = -10; i <= 10; i++) {
            // Horizontal lines
            glVertex3f(-10.0f, 0.0f, (float)i);
            glVertex3f(10.0f, 0.0f, (float)i);
            // Vertical lines
            glVertex3f((float)i, 0.0f, -10.0f);
            glVertex3f((float)i, 0.0f, 10.0f);
        }
        glEnd();
        
        // Draw some colored cubes
        renderCube(-2.0f, 1.0f, -2.0f, 1.0f, 0.0f, 0.0f); // Red cube
        renderCube(2.0f, 1.0f, -2.0f, 0.0f, 1.0f, 0.0f);  // Green cube
        renderCube(0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f);   // Blue cube
        
        // Draw coordinate axes
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); // X axis - red
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(3.0f, 0.0f, 0.0f);
        
        glColor3f(0.0f, 1.0f, 0.0f); // Y axis - green
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 3.0f, 0.0f);
        
        glColor3f(0.0f, 0.0f, 1.0f); // Z axis - blue
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 3.0f);
        glEnd();
    }
    
    void renderCube(float x, float y, float z, float r, float g, float b) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glColor3f(r, g, b);
        
        float size = 0.5f;
        
        glBegin(GL_QUADS);
        // Front face
        glVertex3f(-size, -size,  size);
        glVertex3f( size, -size,  size);
        glVertex3f( size,  size,  size);
        glVertex3f(-size,  size,  size);
        
        // Back face
        glVertex3f(-size, -size, -size);
        glVertex3f(-size,  size, -size);
        glVertex3f( size,  size, -size);
        glVertex3f( size, -size, -size);
        
        // Top face
        glVertex3f(-size,  size, -size);
        glVertex3f(-size,  size,  size);
        glVertex3f( size,  size,  size);
        glVertex3f( size,  size, -size);
        
        // Bottom face
        glVertex3f(-size, -size, -size);
        glVertex3f( size, -size, -size);
        glVertex3f( size, -size,  size);
        glVertex3f(-size, -size,  size);
        
        // Right face
        glVertex3f( size, -size, -size);
        glVertex3f( size,  size, -size);
        glVertex3f( size,  size,  size);
        glVertex3f( size, -size,  size);
        
        // Left face
        glVertex3f(-size, -size, -size);
        glVertex3f(-size, -size,  size);
        glVertex3f(-size,  size,  size);
        glVertex3f(-size,  size, -size);
        glEnd();
        
        glPopMatrix();
    }
    
    void renderUI() {
        // Switch to 2D rendering for UI
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, mWidth, mHeight, 0, -1, 1);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glDisable(GL_DEPTH_TEST);
        
        // Draw simple text-like indicators (using basic shapes)
        glColor3f(1.0f, 1.0f, 1.0f);
        
        // FPS indicator (simple rectangle)
        glBegin(GL_QUADS);
        glVertex2f(10, 10);
        glVertex2f(60, 10);
        glVertex2f(60, 20);
        glVertex2f(10, 20);
        glEnd();
        
        // Position indicator
        glBegin(GL_QUADS);
        glVertex2f(10, 30);
        glVertex2f(150, 30);
        glVertex2f(150, 40);
        glVertex2f(10, 40);
        glEnd();
        
        // Instructions
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(10, mHeight - 80);
        glVertex2f(400, mHeight - 80);
        glVertex2f(400, mHeight - 10);
        glVertex2f(10, mHeight - 10);
        glEnd();
        
        glEnable(GL_DEPTH_TEST);
        
        // Restore matrices
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
    
    void run() {
        if (!initialize()) {
            return;
        }
        
        mRunning = true;
        
        std::cout << "\nArena Controls:" << std::endl;
        std::cout << "  WASD - Move camera" << std::endl;
        std::cout << "  Space/Shift - Move up/down" << std::endl;
        std::cout << "  Mouse - Look around" << std::endl;
        std::cout << "  F1 - Toggle mouse capture" << std::endl;
        std::cout << "  ESC - Exit" << std::endl;
        std::cout << "\nStarting main loop..." << std::endl;
        
        SDL_Event event;
        Uint32 lastTime = SDL_GetTicks();
        int frameCount = 0;
        
        while (mRunning) {
            // Handle events
            while (SDL_PollEvent(&event)) {
                handleEvent(event);
            }
            
            // Update
            update();
            
            // Render
            render();
            
            frameCount++;
            
            // Simple FPS display every second
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastTime >= 1000) {
                std::cout << "FPS: " << frameCount << " | Camera: (" 
                         << mCameraX << ", " << mCameraY << ", " << mCameraZ << ")" << std::endl;
                frameCount = 0;
                lastTime = currentTime;
            }
        }
        
        std::cout << "Arena shutting down..." << std::endl;
    }
};

int main(int argc, char* argv[]) {
    (void)argc; // Suppress unused parameter warning
    (void)argv;
    
    std::cout << "AtlasTech2 Minimal Arena Starting..." << std::endl;
    
    MinimalArena arena;
    arena.run();
    
    std::cout << "AtlasTech2 Minimal Arena Exited Successfully" << std::endl;
    return 0;
}