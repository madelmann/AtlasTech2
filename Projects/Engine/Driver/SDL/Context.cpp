
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
  mWindowHandle(0)
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

	return true;
}

void Context::destroySDLContext()
{
	SDL_Quit();
}

const Context::Settings& Context::provideSettings() const
{
	return mSettings;
}

bool Context::setup(HWND hwnd, const Settings& s)
{
	// set window handle
	mWindowHandle = hwnd;

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
