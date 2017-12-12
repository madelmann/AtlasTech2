
// Header
#include "Console.h"

// Library includes

// Project includes
#include "ConsolePlugin.h"
#include <Console/Plugins/CLS.h>
#include <Console/Plugins/Echo.h>
#include <Console/Plugins/Help.h>
#include <Console/Plugins/Man.h>
#include <Console/Plugins/Quit.h>
#include <Core/EngineSettings.h>
#include <Font/Font.h>
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Parsers/Valueizer.h>
#include <Renderer/IRenderer.h>
#include <Tools/Strings.h>

// Namespace declarations


namespace Console {


const char ConsoleCursor = '_';
const char ConsoleInputChar = '>';

const std::string Command_Comment = "//";
const std::string Command_Use = "use";
const std::string Command_UnUse = "..";


Console::Console(const Common::Logger *p, HDC hdc)
: Common::Logger(p, "Console"),
  mAutocompleteIndex(0),
  mCursorX(0),
  mCursorY(0),
  mEngineSettings(0),
  mFont(0),
  mHdc(hdc),
  mMediaPathManager(0),
  mRenderer(0),
  mTextureManager(0),
  mZero(-0.96f, 0.92f)
{
	debug("Starting...");

	mColor = vector3f(1.f, 1.f, 1.f);
	mIsContainer = false;
	mName = "screen.console";
}

Console::~Console()
{
	shutdown();
}

void Console::autocomplete()
{
	unsigned int len = mCommand.length();

	for ( StringList::iterator it = mAutocompleteData.begin(); it != mAutocompleteData.end(); it++ ) {
		std::string tmp = (*it);

		if ( tmp.length() < len ) {
			// command is longer than autocomplete string - continue...
			continue;
		}

		// resize autocomplete string to fit with our command
		tmp.resize(len);

		if ( Tools::StringCompareI(mCommand, tmp) ) {
			// fill command with autocomplete string
			mCommand = (*it);
			return;
		}
	}
}

void Console::clear()
{
	mSettings.clear();

	mOutputBuffer.clear();
}

void Console::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mEngineSettings);
	assert(mMediaPathManager);
	assert(mRenderer);
	assert(mTextureManager);

	mBackground = mTextureManager->create(
		  mMediaPathManager->getSystem()
		+ mMediaPathManager->getConsole()
		+ "console_background.tga"
	);

	mFont = new FontSpace::Font();
	mFont->connectTextureManager(mTextureManager);
	mFont->configurationComplete();

	mFont->setWidthHeight(mEngineSettings->getScreen()->getWidth(), mEngineSettings->getScreen()->getHeight());
	mFont->Initialize(mHdc, mSettings.getFontSize(), mSettings.getFontName());

	assert(mBackground);

	init();

	mConfigurationComplete = true;
	mIsReady = true;
}

void Console::connectEngineSettings(Core::EngineSettings *e)
{
	mEngineSettings = e;
}

void Console::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void Console::connectRenderer(Core::IRenderer *r)
{
	mRenderer = r;
}

void Console::connectTextureManager(TextureSpace::IManager *t)
{
	mTextureManager = t;
}

void Console::disable()
{
	mSettings.enabled(false);
}

void Console::enable()
{
	mSettings.enabled(true);
}

void Console::execute(const std::string& command)
{
	mCommand = command;

	processCommand();
}

void Console::hide()
{
	UIScreen::hide();
}

void Console::init()
{
	setBackground(mBackground);
	setColor(vector3f(1.f, 1.f, 1.f));

	// Add default plugins
	// ConsolePlugin
	ConsolePlugin *consolePlugin = new ConsolePlugin();
	consolePlugin->connectConsole(this);

	registerPlugin(consolePlugin);

	// Simple plugins
	registerPlugin(new CLS());
	registerPlugin(new Echo());
	registerPlugin(new Quit());

	// 'Complexer' plugins
	Help *help = new Help();
	help->connectMediaPathManager(mMediaPathManager);
	registerPlugin(help);

	Man *man = new Man();
	man->connectMediaPathManager(mMediaPathManager);
	registerPlugin(man);

	// Load autocomplete list from file
	FILE *pFile;
	std::string filename = mMediaPathManager->getGame()
						 + mMediaPathManager->getSystem()
						 + mMediaPathManager->getConsole()
						 + "autocomplete.txt";

	fopen_s(&pFile, filename.c_str(), "rt");
	if ( !pFile ) {
		warn("Could not load auto complete data!");
		return;
	}

	char oneline[255];

	do {
		sprintf(oneline, "");
		Tools::readline(pFile, oneline);

		mAutocompleteData.push_back(std::string(oneline));
	} while ( strcmp(oneline, "") != 0 );

	fclose(pFile);
}

void Console::insertChar(unsigned int idx, int c)
{
	if ( idx > mCommand.length() ) {
		mCommand += static_cast<char>(tolower(c));
	}
	else {
		std::string::iterator it = mCommand.begin() + idx;
		mCommand.insert(it, static_cast<char>(tolower(c)));
	}
}

void Console::print(const std::string& text)
{
	// Print to log
	//debug("print: '" + text + "'");
	info(text);

	// Print to screen
	mOutputBuffer.push_back(text);

	if ( mOutputBuffer.size() > mSettings.getVisibleLines() ) {
		mOutputBuffer.pop_front();
	}
}

void Console::print(const std::string& text, int line, int column)
{
assert(!"implement me!");

(void)line;
(void)column;
	print(text);
}

void Console::processCommand()
{
	if ( mSettings.getEcho() ) {
		print(mCommand);
	}

	if ( mCommand.empty() ) {
		return;
	}

	mCommandBuffer.push_back(mCommand);

	bool executed = false;

	Valueizer t((mProgramNamespace.empty() ? "" : mProgramNamespace + " ") + mCommand);
	if ( t.hasNext() ) {
		// Internal commands
		if ( t.getToken() == Command_Comment ) {
			executed = true;
		}
		else if ( t.getToken() == Command_Use ) {
			if ( t.hasNext() ) {
				Value v = t.getTokenAsValue();
				mProgramNamespace = v.getValue();
			}
			executed = true;
		}
		else if ( mCommand == Command_UnUse ) {
			mProgramNamespace =  "";
			executed = true;
		}
		// Plugin functionality
		else {
			std::string cmd = mProgramNamespace.empty() ? t.getToken() : mProgramNamespace;

			for ( PluginList::iterator it = mConsolePlugins.begin(); it != mConsolePlugins.end(); it++ ) {
				if ( Tools::StringCompareI(cmd, (*it)->name()) ) {
					(*it)->execute(&t);
					executed = true;
					break;
				}
			}
		}
	}

	if ( !executed ) {
		print("command '" + t.getToken() + "' could not be executed!");
	}

	resetCommand();
}

bool Console::onKeyDown(WPARAM key)
{
	switch ( key ) {
		case VK_BACK:
			if ( mCursorX > 0 ) {
				removeChar(mCursorX - 1);
				mCursorX--;
			}
			break;
		case VK_ESCAPE:
			resetCommand();
			break;
		case VK_DELETE:
			removeChar(mCursorX);
			break;
		case VK_DOWN:
			mCommand = mCommandBuffer.up();
			break;
		case VK_END:
			mCursorX = mCommand.length();
			break;
		case VK_F9:
			hide();
			break;
		case VK_HOME:
			mCursorX = 0;
			break;
		case VK_LEFT:
			if ( mCursorX > 0 ) {
				mCursorX--;
			}
			break;
		case VK_RETURN:
			processCommand();
			break;
		case VK_RIGHT:
			mCursorX++;
			if ( mCursorX > mCommand.length() ) {
				mCursorX = mCommand.length();
			}
			break;
		case VK_TAB:
			autocomplete();
			break;
		case VK_UP:
			mCommand = mCommandBuffer.down();
			break;
		case 188:
			insertChar(mCursorX, ',');
			mCursorX++;
			break;
		case 190:
			insertChar(mCursorX, '.');
			mCursorX++;
			break;
		default:
			insertChar(mCursorX, static_cast<int>(key));
			mCursorX++;
			break;
	}

	return true;
}

Settings* Console::provideSettings()
{
	return &mSettings;
}

void Console::registerPlugin(IConsolePlugin *plugin)
{
	debug("registerPlugin('" + plugin->name() + "')");

	// Connect printer
	plugin->connectPrinter(this);
	plugin->connectSettings(&mSettings);

	mConsolePlugins.push_back(plugin);
}

void Console::removeChar(unsigned int idx)
{
	if ( idx >= mCommand.length() ) {
		return;
	}

	std::string::iterator it;
	it = mCommand.begin() + idx;

	mCommand.erase(it);
}

void Console::render()
{
	if ( mSettings.isBackgroundVisible() ) {
		UIScreen::render();
	}

	unsigned int lines = mSettings.getVisibleLines() - mOutputBuffer.size();
	for ( StringList::iterator it = mOutputBuffer.begin(); it != mOutputBuffer.end(); it++, lines++ ) {
		mFont->PrintText((*it), mZero.x, mZero.y - lines * mSettings.getLineSpace());
	}

	mFont->PrintText(mProgramNamespace + ConsoleInputChar + mCommand, mZero.x, mZero.y - mSettings.getVisibleLines() * mSettings.getLineSpace());
	mFont->PrintText(&ConsoleCursor, mZero.x + mFont->getCharWidth() * (mProgramNamespace.length() + mCursorX + 1), mZero.y - mSettings.getVisibleLines() * mSettings.getLineSpace());
}

void Console::resetCommand()
{
	mCommand = "";
	mCursorX = mCommand.length();
}

void Console::show()
{
	if ( !mSettings.enabled() ) {
		return;
	}

	UIScreen::show();
}

void Console::shutdown()
{
	debug("Stopping...");

	mCommandBuffer.clear();
	mOutputBuffer.clear();

	for ( PluginList::iterator it = mConsolePlugins.begin(); it != mConsolePlugins.end(); it++ ) {
		delete (*it);
	}

	delete mFont;
}


}
