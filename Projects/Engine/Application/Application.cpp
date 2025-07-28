// Header
#include "Application.h"

// Library includes
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

// Project includes
#include <Driver/DriverUtils.h>
#include <Misc/Colors.h>

// Namespace declarations


namespace Core {


Application::Application(const std::string& name)
: Common::Logger(0, ""),
  mConsole(0),
  mEngine(0),
  mEngineSettings(0),
  mGeneralManager(0),
  mIdGenerator(0),
  mKeyboard(0),
  mMediaPathManager(0),
  mMouse(0),
  mName(name),
  mRunning(false),
  mSceneManager(0),
  mScreenManager(0),
  mScreenWriter(0),
  mShowDebugInterface(true),
  mSoundManager(0),
  mWindow(nullptr)
{
	//debug("Starting...");

	mEngine = new Engine(this);
	mEngineSettings = mEngine->provideEngineSettings();
}

Application::~Application()
{
	//debug("Stopping...");

	// prevent double delete
	if (mScreenManager) {
		mScreenManager->remove(this);
	}

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
	
	mRunning = true;
	SDL_Event event;

	while ( keepAlive() && mRunning ) {
		// Handle SDL events
		while ( SDL_PollEvent(&event) ) {
			if ( !handleSDLEvent(event) ) {
				stop();
				mRunning = false;
				return;
			}
		}

		mEngine->manage();

		if (mScreenManager) {
			mScreenManager->update(mEngine->getFrameTime());
		}

		// Display debug interface
		if ( mShowDebugInterface && mScreenWriter ) {
			static char text[255] = "";

			sprintf(text, "FPS: %.0f", mEngine->getFrameRate());
			mScreenWriter->write(-0.97f, 0.93f, text, 0, Color::red);

			std::string gameMode = "Pause";
			if ( mEngine->isRunning() ) {
				gameMode = "Play";
			}
			mScreenWriter->write(0.86f, 0.93f, gameMode, 0, Color::yellow);

			// Mouse
			if (mMouse) {
				vector2l m2d = mMouse->getPosition2D();
				vector3f m3d = mMouse->getPosition3D();

				sprintf(text, "X = %i, Y = %i", m2d.x, m2d.y);
				mScreenWriter->write(-0.97f, 0.87f, text, 0, Color::blue);
				sprintf(text, "X = %.2f, Y = %.2f, Z = %.2f", m3d.x, m3d.y, m3d.z);
				mScreenWriter->write(-0.97f, 0.79f, text, 0, Color::blue);
			}
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

bool Application::handleSDLEvent(const SDL_Event& event)
{
	switch ( event.type ) {
		case SDL_QUIT:
			return false;
			
		case SDL_WINDOWEVENT:
			if ( event.window.event == SDL_WINDOWEVENT_RESIZED ) {
				if ( mEngine ) {
					mEngine->reshape(
						0.0f,
						0.0f,
						(float)event.window.data1,
						(float)event.window.data2
					);
				}
			}
			else if ( event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED ) {
				if ( mEngine && mEngine->pauseWhileInactive() ) {
					mEngine->Play();
				}
			}
			else if ( event.window.event == SDL_WINDOWEVENT_FOCUS_LOST ) {
				if ( mEngine && mEngine->pauseWhileInactive() ) {
					mEngine->Pause();
				}
			}
			break;
			
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			// Handle key events - these would need to be converted to your engine's key format
			// For now, just pass through to the engine if it has key handling
			break;
			
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			// Handle mouse events - these would need to be converted to your engine's mouse format
			break;
	}
	
	return true;
}

SDL_Window* Application::window() const
{
	return mWindow;
}

void Application::initialize()
{
	debug("Initializing...");

	mEngine->connectProcessHandler(this);

	mWindow = mEngine->createClientWindow(this);
	if ( mWindow == nullptr ) {
		error("could not create window!");
		return;
	}

	// Connect to all managers
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

	// Show the window
	if (mWindow) {
		SDL_ShowWindow(mWindow);
	}

	// Finish initialization
	mEngine->initialize();

	// Add ourself to the screen manager
	if (mScreenManager) {
		mScreenManager->add(this);
	}

	// Start our process
	processStart(this);
}

const std::string& Application::name() const
{
	return mName;
}

void Application::update(float elapsedTime)
{
	// Default implementation - can be overridden by derived classes
}


}