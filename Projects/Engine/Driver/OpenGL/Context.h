
#ifndef _Driver_OpenGL_Context_h_
#define _Driver_OpenGL_Context_h_


// MS pragmas
// #pragma comment(lib, "opengl32.lib")

// Library includes
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

// Project includes
#include <Common/Logger.h>

// Forward declarations

// Namespace declarations


namespace OpenGL {


class Context : private Common::Logger
{
public:
	class Settings
	{
	public:
		int	mColorDepth;
		int mHeight;
		int	mWidth;
	};

public:
	Context(const Common::Logger *p);
	virtual ~Context();

public:
	bool change(const Settings& s);
	const Settings& provideSettings() const;
	bool setup(HWND hwnd, const Settings& s);

protected:
	virtual bool createOpenGLContext();
	virtual void destroyOpenGLContext();

private:
	void shutdown();
	void startup();

private:
	HDC			mDeviceContext;
	HGLRC		mRenderingContext;
	Settings	mSettings;
	HWND		mWindowHandle;
};


}


#endif
