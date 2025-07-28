
// Header
#include "Context.h"

// Library includes
#include <iostream>

// Project includes

// Namespace declarations


namespace SDL {


Context::Context(const Common::Logger *p)
: Common::Logger(p, "SDL::Context"),
  mWindow(nullptr),
  mGLContext(nullptr)
{
}

Context::~Context()
{
	shutdown();
}

bool Context::change(const Settings& s)
{
	// validate & execute changes ...
	
	// For now, we need to recreate the window for resolution changes
	// A more sophisticated implementation would handle some changes without recreation
	if (mWindow && (mSettings.mWidth != s.mWidth || mSettings.mHeight != s.mHeight)) {
		shutdown();
		mSettings = s;
		return createSDLContext();
	}

	// ... and update our settings
	mSettings = s;

	return true;
}

bool Context::createSDLContext()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		error("SDL_Init failed: " + std::string(SDL_GetError()));
		return false;
	}

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	// Create window
	mWindow = SDL_CreateWindow(
		"AtlasTech2 Arena",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		mSettings.mWidth,
		mSettings.mHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	if (!mWindow) {
		error("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
		SDL_Quit();
		return false;
	}

	// Create OpenGL context
	mGLContext = SDL_GL_CreateContext(mWindow);
	if (!mGLContext) {
		error("SDL_GL_CreateContext failed: " + std::string(SDL_GetError()));
		SDL_DestroyWindow(mWindow);
		mWindow = nullptr;
		SDL_Quit();
		return false;
	}

	// Enable VSync
	if (SDL_GL_SetSwapInterval(1) < 0) {
		warn("Warning: Unable to set VSync: " + std::string(SDL_GetError()));
	}

	debug("SDL Context created successfully");
	return true;
}

void Context::destroySDLContext()
{
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

const Context::Settings& Context::provideSettings() const
{
	return mSettings;
}

bool Context::setup(SDL_Window* window, const Settings& s)
{
	// If a window is provided, use it; otherwise create our own
	if (window) {
		mWindow = window;
		// We still need to create the GL context
		mGLContext = SDL_GL_CreateContext(mWindow);
		if (!mGLContext) {
			error("SDL_GL_CreateContext failed: " + std::string(SDL_GetError()));
			return false;
		}
	} else {
		// set our opengl context settings
		mSettings = s;
		
		// initialize
		return startup();
	}

	mSettings = s;
	return true;
}

SDL_Window* Context::getWindow() const
{
	return mWindow;
}

SDL_GLContext Context::getGLContext() const
{
	return mGLContext;
}

void Context::shutdown()
{
	destroySDLContext();
}

bool Context::startup()
{
	return createSDLContext();
}


}
