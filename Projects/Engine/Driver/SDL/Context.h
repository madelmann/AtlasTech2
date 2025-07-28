
#ifndef _Driver_SDL_Context_h_
#define _Driver_SDL_Context_h_


// Library includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Project includes
#include <Common/Logger.h>

// Forward declarations

// Namespace declarations


namespace SDL {


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
	bool setup(SDL_Window* window, const Settings& s);
	SDL_Window* getWindow() const;
	SDL_GLContext getGLContext() const;

protected:
	virtual bool createSDLContext();
	virtual void destroySDLContext();

private:
	void shutdown();
	void startup();

private:
	SDL_Window*		mWindow;
	SDL_GLContext	mGLContext;
	Settings		mSettings;
};


}


#endif
