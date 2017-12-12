
#ifndef _Driver_OpenGL_Context_h_
#define _Driver_OpenGL_Context_h_


// MS pragmas
//#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

// Library includes
#include <windows.h>
#include <OpenGL/include/gl.h>										// Header File For The OpenGL32 Library
//#include <OpenGL/include/glext.h>
//#include <OpenGL/include/glu.h>										// Header File For The GLu32 Library
//#include <OpenGL/include/glut.h>									// Header file for the opengl utility library
//#include <OpenGL/include/glaux.h>									// Header File For The GLaux Library

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
	virtual void render();

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
