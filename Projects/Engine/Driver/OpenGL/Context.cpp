
// Header
#include "Context.h"

// Library includes

// Project includes

// Namespace declarations


namespace OpenGL {


Context::Context(const Common::Logger *p)
: Common::Logger(p, "OpenGL::Context"),
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

bool Context::createOpenGLContext()
{
	mDeviceContext = GetDC(mWindowHandle);
	if ( !mDeviceContext ) {
		return false;
	}

	return true;
}

void Context::destroyOpenGLContext()
{
	wglMakeCurrent(mDeviceContext, 0);
	wglDeleteContext(mRenderingContext);

	ReleaseDC(mWindowHandle, mDeviceContext);
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
	destroyOpenGLContext();
}

void Context::startup()
{
	createOpenGLContext();
}


}
