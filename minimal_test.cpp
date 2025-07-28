// Minimal test program to verify SDL compilation and basic functionality
// This program initializes SDL, creates a window, and exits cleanly

#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

int main(int argc, char* argv[])
{
    std::cout << "AtlasTech2 Minimal Test Starting..." << std::endl;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    std::cout << "SDL initialized successfully" << std::endl;

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "AtlasTech2 Minimal Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    std::cout << "Window created successfully" << std::endl;

    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    std::cout << "OpenGL context created successfully" << std::endl;

    // Get OpenGL version
    const GLubyte* glVersion = glGetString(GL_VERSION);
    const GLubyte* glVendor = glGetString(GL_VENDOR);
    const GLubyte* glRenderer = glGetString(GL_RENDERER);

    std::cout << "OpenGL Version: " << glVersion << std::endl;
    std::cout << "OpenGL Vendor: " << glVendor << std::endl;
    std::cout << "OpenGL Renderer: " << glRenderer << std::endl;

    // Simple render test
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);

    std::cout << "Rendered blue screen successfully" << std::endl;

    // Wait a moment so user can see the window
    SDL_Delay(2000);

    // Event loop test (brief)
    SDL_Event event;
    bool running = true;
    int frameCount = 0;

    std::cout << "Starting brief event loop test (5 seconds)..." << std::endl;

    Uint32 startTime = SDL_GetTicks();
    while (running && (SDL_GetTicks() - startTime) < 5000) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        // Simple animation - change clear color
        float time = (SDL_GetTicks() - startTime) / 1000.0f;
        float r = (sin(time * 2.0f) + 1.0f) * 0.5f;
        float g = (sin(time * 3.0f) + 1.0f) * 0.5f;
        float b = (sin(time * 5.0f) + 1.0f) * 0.5f;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);

        frameCount++;
        SDL_Delay(16); // ~60 FPS
    }

    std::cout << "Event loop completed. Rendered " << frameCount << " frames." << std::endl;

    // Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "AtlasTech2 Minimal Test completed successfully!" << std::endl;
    return 0;
}