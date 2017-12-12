
// Header
#include "Application.h"

// Library includes
#include <iostream>
#include <fstream>

// Project includes
#include <Driver/DriverUtils.h>
#include <Misc/Colors.h>

// Namespace declarations


extern LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


namespace Core {


Application::Application(const std::string& name, HINSTANCE instance)
: Common::Logger(0, ""),
  mConsole(0),
  mEngine(0),
  mEngineSettings(0),
  mGeneralManager(0),
  mHWND(0),
  mIdGenerator(0),
  mInstance(instance),
  mKeyboard(0),
  mMediaPathManager(0),
  mMouse(0),
  mName(name),
  mSceneManager(0),
  mScreenManager(0),
  mScreenWriter(0),
  mShowDebugInterface(true),
  mSoundManager(0)
{
	//debug("Starting...");

	mEngine = new Engine(this);
	mEngineSettings = mEngine->provideEngineSettings();
}

Application::~Application()
{
	//debug("Stopping...");

	// prevent double delete
	mScreenManager->remove(this);

	aeDelete( mEngine );
}

void Application::autoexec()
{
	debug("processing 'game.autoexec'...");

	std::string filename = mMediaPathManager->buildPath("game.autoexec");
	std::ifstream file(filename.c_str());

	if ( file.is_open() ) {
		while ( file.good() ) {
			std::string line;
			getline(file, line);

			mConsole->execute(line);
		}
		file.close();
	}
}

void Application::exec()
{
	autoexec();

	static MSG msg;

	while ( keepAlive() ) {
		if ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) ) {
			if ( msg.message == WM_QUIT ) {
				stop();
				return;
			}
			else {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}

		mEngine->manage();

		mScreenManager->update(mEngine->getFrameTime());

		// Display debug interface
		if ( mShowDebugInterface ) {
			static char text[255] = "";

			sprintf(text, "FPS: %.0f", mEngine->getFrameRate());
			mScreenWriter->write(-0.97f, 0.93f, text, 0, Color::red);

			std::string gameMode = "Pause";
			if ( mEngine->isRunning() ) {
				gameMode = "Play";
			}
			mScreenWriter->write(0.86f, 0.93f, gameMode, 0, Color::yellow);

			// Mouse
			// {
			vector2l m2d = mMouse->getPosition2D();
			vector3f m3d = mMouse->getPosition3D();

			sprintf(text, "X = %i, Y = %i", m2d.x, m2d.y);
			mScreenWriter->write(-0.97f, 0.87f, text, 0, Color::blue);
			sprintf(text, "X = %.2f, Y = %.2f, Z = %.2f", m3d.x, m3d.y, m3d.z);
			mScreenWriter->write(-0.97f, 0.79f, text, 0, Color::blue);
			// }
		}

		// run derived application implementation
		run();

		// run "child"-processes
		runProcesses();


		assert(aeGetAttributesLeft() == 0);
		assert(aeGetImmediateModeLeft() == 0);
		assert(aeGetMatricesLeft() == 0);
	}
}

LRESULT CALLBACK Application::handle(const Common::Event &e, HWND hwnd)
{
	if ( hwnd != this->hwnd() ) {
		return DefWindowProc(hwnd, e.uMsg, e.wParam, e.lParam); 				// Return the default
	}

	static PAINTSTRUCT ps;

	switch ( e.uMsg ) {
		case WM_ACTIVATE:
			if ( mEngine && mEngine->pauseWhileInactive() ) {
				switch ( e.wParam ) {
					case WA_ACTIVE:
					case WA_CLICKACTIVE:
						mEngine->Play();
						break;
					case WA_INACTIVE:
						mEngine->Pause();
						break;
				}
			}
			break;
		case WM_CLOSE:															// If the window is closed
			PostQuitMessage(0);													// Tell windows we want to quit
			break;
		case WM_KEYDOWN:
			if ( mScreenManager ) {
				static UIScreen *console = mScreenManager->get("screen.console");
				if ( console ) {
					if ( console->isVisible() ) {
						console->onKeyDown(e.wParam);
						return true;
					}
					else if ( mKeyboard && mKeyboard->getKey("console")->getKeyCode() == e.wParam ) {
						mScreenManager->show(console);
						return true;
					}
				}
			}
			break;
		case WM_MOVE:
		case WM_SIZE:
			if ( mEngine ) {
				RECT rcClient;//, rcScreen;
				GetClientRect(hwnd, &rcClient);									// Get the window rectangle
				//ClientRectToScreen(mHWND, &rcClient, &rcScreen);

				mEngine->reshape(
					(float)rcClient.left,
					(float)rcClient.top,
					(float)rcClient.right - rcClient.left,
					(float)rcClient.bottom - rcClient.top
				);
			}
			break;
		case WM_PAINT:															// If we need to repaint the scene
			BeginPaint(mHWND, &ps);												// Init the paint struct		
			EndPaint(mHWND, &ps);												// EndPaint, Clean up
			break;
		case WM_SYSCOMMAND:	{													// Intercept System Commands
			switch ( e.wParam ) {
				case SC_MONITORPOWER:											// Monitor Trying To Enter Powersave?
				case SC_SCREENSAVE:												// Screensaver Trying To Start?
					return false;
			}
		} break;
	}

	if ( mScreenManager && mScreenManager->handleEvent(e) ) {
		return true;
	}

	for ( ProcessList::iterator it = mProcesses.begin(); it != mProcesses.end(); ++it ) {
		if ( (*it)->isRunning() && (*it)->handleEvent(e) ) {
			return true;
		}
	}

	return DefWindowProc(hwnd, e.uMsg, e.wParam, e.lParam); 					// Return the default
}

HWND Application::hwnd() const
{
	return mEngine->hwnd();
}

void Application::initialize()
{
	debug("Initializing...");

	mEngine->connectProcessHandler(this);

	mHWND = mEngine->createClientWindow(this);
	if ( mHWND == 0 ) {
		error("could not create window handle!");
		return;
	}

	mEngine->initialize();

	// Setup our objects
	mConsole = mEngine->provideConsole();
	mGeneralManager = mEngine->provideGeneralManager();
	mIdGenerator = mEngine->provideIdGenerator();
	mKeyboard = mEngine->provideKeyboard();
	mMediaPathManager = mEngine->provideMediaPathManager();
	mMouse = mEngine->provideMouse();
	mSceneManager = mEngine->provideSceneManager();
	mScreenManager = mEngine->provideScreenManager();
	mScreenWriter = mEngine->provideScreenWriter();
	mSoundManager = mEngine->provideSoundManager();

	this->setFont(mEngine->provideFont());
	this->setName("screen.main");
	this->init();

	mScreenManager->add(this);
	mScreenManager->show("screen.main");


	mKeyboard->addKey(VK_ESCAPE,	"escape");
	mKeyboard->addKey(VK_F5,		"play");
	mKeyboard->addKey(VK_F6,		"freecamera");
	mKeyboard->addKey(VK_F9,		"console");
	mKeyboard->addKey(VK_RETURN,	"return");


	processStart(this);
}

HINSTANCE Application::instance() const
{
	return mInstance;
}

const std::string& Application::name() const
{
	return mName;
}

void Application::update(float /*elapsedTime*/)
{
	//mScreenManager->update(elapsedTime);
}


}
