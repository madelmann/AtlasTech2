
// Header
#include "Context.h"

// Library includes

// Project includes

// Namespace declarations


namespace SDL {


Context::Context(const Common::Logger *p)
: Common::Logger(p, "SDL::Context"),
  mDeviceContext(0),
  mRenderingContext(0),
  mWindow(0)
{
}

Context::~Context()
{
	shutdown();
}

bool Context::change(const Settings& s)
{
	// validate & execute changes ...

	// ... and update our settings
	mSettings = s;

	return true;
}

bool Context::createSDLContext()
{
	if ( SDL_Init(SDL_INIT_VIDEO) != 0 ){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
	mWindow = SDL_CreateWindow("SDL2/OpenGL Demo", 0, 0, 640, 480, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

	// Create an OpenGL context associated with the window.
	mDeviceContext = SDL_GL_CreateContext(mWindow);

	return true;
}

void Context::destroySDLContext()
{
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(mDeviceContext);  

	// Destroy window after we are done
	SDL_DestroyWindow(mWindow);

	SDL_Quit();
}

const Context::Settings& Context::provideSettings() const
{
	return mSettings;
}

bool Context::setup(const Settings& s)
{
	// set our opengl context settings
	mSettings = s;

	// initialize
	startup();

	return true;
}

void Context::shutdown()
{
	destroySDLContext();
}

void Context::startup()
{
	createSDLContext();
}


}
