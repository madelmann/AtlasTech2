
#ifndef _Console_Console_h_
#define _Console_Console_h_


// Library includes
#include <list>
#include <string>
#include <windows.h>

// Project includes
#include "Settings.h"
#include <Common/Logger.h>
#include <Interfaces/Common/AComponent.h>
#include <Interfaces/Common/Containers.h>
#include <Interfaces/Console/IConsole.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Math/Vector2.h>
#include <UIKit/UIScreen.h>

// Forward declarations
namespace Core {
	class EngineSettings;
	class IRenderer;
}
namespace FontSpace {
	class Font;
}
class IMediaPathManager;
namespace TextureSpace {
	class IManager;
	class Texture;
}

// Namespace declarations
using TextureSpace::Texture;


namespace Console {

// Forward declarations
class IConsolePlugin;

class CommandBuffer
{
public:
	void clear() {
		mBuffer.clear();
		mIndex = mBuffer.end();
	}
	void push_back(const std::string& cmd) {
		mBuffer.push_back(cmd);
		mIndex = mBuffer.end();
	}

	std::string down() {
		if ( mBuffer.empty() ) {
			return "";
		}

		if ( mIndex != mBuffer.begin() ) {
			mIndex--;
		}

		return (*mIndex);
	}
	std::string up() {
		if ( mBuffer.empty() ) {
			return "";
		}

		if ( mIndex != mBuffer.end() ) {
			mIndex++;
		}
		if ( mIndex == mBuffer.end() ) {
			mIndex--;
		}

		return (*mIndex);
	}

private:
	StringList	mBuffer;
	StringList::iterator	mIndex;
};


class Console : public Common::AComponent,
				public IConsole,
				public UIKit::UIScreen,
				private Common::Logger
{
public:
	Console(const Common::Logger *p, HDC hdc);
	~Console();

public:
	// Common::AComponent implementation
	//{
	void configurationComplete();
	void shutdown();
	//}

	// ICommandExecutor implementation
	// {
	void execute(const std::string& command);
	// }

	// IPrinter implementation
	//{
	void clear();
	void print(const std::string& text);
	void print(const std::string& text, int line, int column);
	//}

	// Connectors
	//{
	void connectEngineSettings(Core::EngineSettings *e);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectRenderer(Core::IRenderer *r);
	void connectTextureManager(TextureSpace::IManager *m);
	//}

	// Providers
	//{
	Settings* provideSettings();
	//}

	// IConsole implementation
	//{
	void registerPlugin(IConsolePlugin *plugin);

	void disable();
	void enable();
	void hide();
	void show();
	//}

	// UIKit::UIScreen
	// {
	bool onKeyDown(WPARAM key);
	void render();
	// }

	void processCommand();

protected:

private:
	void autocomplete();
	void init();
	void resetCommand();

	void insertChar(unsigned int idx, int c);
	void removeChar(unsigned int idx);

private:
	typedef std::list<IConsolePlugin*> PluginList;

private:
	Core::EngineSettings	*mEngineSettings;
	FontSpace::Font			*mFont;
	IMediaPathManager		*mMediaPathManager;
	Core::IRenderer			*mRenderer;
	Settings				mSettings;
	TextureSpace::IManager	*mTextureManager;


	StringList		mAutocompleteData;
	int				mAutocompleteIndex;
	float			mCharHeight;
	float			mCharWidth;
	std::string		mCommand;
	CommandBuffer	mCommandBuffer;
	unsigned int	mCursorX;
	unsigned int	mCursorY;
	PluginList		mConsolePlugins;
	HDC				mHdc;
	StringList		mOutputBuffer;
	std::string		mProgramNamespace;
	vector2f		mZero;
};


}


#endif
