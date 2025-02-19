
#ifndef _Driver_SDL_Context_h_
#define _Driver_SDL_Context_h_


// Library includes
#include <SDL/include/SDL.h>

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
	bool setup(const Settings& s);

protected:
	virtual bool createSDLContext();
	virtual void destroySDLContext();

private:
	void shutdown();
	void startup();

private:
	SDL_GLContext mDeviceContext;
	SDL_GLContext mRenderingContext;
	Settings mSettings;
	SDL_Window* mWindow;
};


}


#endif
