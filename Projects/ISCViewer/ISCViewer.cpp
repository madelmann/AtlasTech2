
// Header
#include "ISCViewer.h"

// Library includes

// Project includes
#include <Camera\MovieCamera\MovieCamera.h>
#include <Driver\DriverUtils.h>
#include <Interfaces\Console\IConsole.h>
#include <Interfaces\Core\Types.h>
#include <Interfaces\Media\IMediaPathManager.h>
#include <Interfaces\Mesh\IObjectManager.h>
#include <Interfaces\Physics\IManager.h>
#include <Interfaces\Scene\IManager.h>
#include <Interfaces\Script\IManager.h>
#include <Interfaces\Terrain\IManager.h>
#include <Interfaces\Texture\IManager.h>
#include <Interfaces\UIKit\IManager.h>
#include <Math\Vector2.h>
#include <ObjectiveScript\Script.h>
#include <Parsers\ConfigParser.h>
#include <Renderer\RenderObject.h>
#include <Scene\SceneGraphObject.h>

// Application includes
#include <Camera\EditorCamera.h>
#include <Console\Plugins\EditorPlugin.h>
#include <Menu\MainMenu.h>
#include <Warehouse\Warehouse.h>

// Namespace declarations


ISCViewer::ISCViewer(HINSTANCE instance)
: Application("ISCViewer", instance),
  mCamera(0),
  mMovieCamera(0),
  mObjectManager(0),
  mParser(0),
  mPhysicManager(0),
  mSceneManager(0),
  mScriptManager(0),
  mTerrainManager(0),
  mWarehouse(0)
{
	debug("Starting...");
}

ISCViewer::~ISCViewer()
{
	debug("Stopping...");

	aeDelete( mCamera );
	aeDelete( mMovieCamera );
	aeDelete( mParser );
	aeDelete( mWarehouse );
}

void ISCViewer::dispatchMouseDown(const Common::Event& e)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}

	switch ( e.uMsg ) {
		case WM_LBUTTONDOWN:
			break;
		case WM_MBUTTONDOWN:
			mCamera->setFollowMouseView(true);
			break;
		case WM_RBUTTONDOWN:
			break;
	}
}

void ISCViewer::dispatchMouseMove(const Common::Event& /*e*/)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}

}

void ISCViewer::dispatchMouseUp(const Common::Event& e)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}

	switch ( e.uMsg ) {
		case WM_LBUTTONUP:
			break;
		case WM_MBUTTONUP:
			mCamera->setFollowMouseView(false);
			break;
		case WM_RBUTTONUP:
			break;
	}
}

void ISCViewer::dispatchMouseWheel(const Common::Event& /*e*/)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}

}

LRESULT ISCViewer::handleEvent(const Common::Event& e)
{
	switch ( e.uMsg ) {
		case WM_KEYDOWN:
			processKeyPress(e.wParam);
			break;
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			dispatchMouseDown(e);
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			dispatchMouseUp(e);
			break;
		case WM_MOUSEMOVE:
			dispatchMouseMove(e);
			break;
		case WM_MOUSEWHEEL:
			dispatchMouseWheel(e);
			break;
	}

	return true;
}

void ISCViewer::initialize(const std::string& cmdline)
{
	debug("Initializing..");

	Application::initialize();
	Application::processAdd(this);


	mCamera = new EditorCamera();
	mMovieCamera = new MovieCamera();
	mObjectManager = mEngine->provideObjectManager();
	mParser = new ConfigParser(this);
	mPhysicManager = mEngine->providePhysicsManager();
	mSceneManager = mEngine->provideSceneManager();
	mScriptManager = mEngine->provideScriptManager();
	mTerrainManager = mEngine->provideTerrainManager();
	mWarehouse = new WarehouseSpace::Warehouse(this);


	mWarehouse->connectObjectManager(mEngine->provideObjectManager());
	mWarehouse->connectSceneManager(mSceneManager);

	mWarehouse->configurationComplete();


	mCamera->connectEngine(mEngine);
	mGeneralManager->add(mMovieCamera);
	mGeneralManager->add(mWarehouse);


	{	// Console plugin setup
		Console::Plugins::EditorPlugin *editor = new Console::Plugins::EditorPlugin();
		editor->connectManagement(mEngine->provideManagement());
		mConsole->registerPlugin(editor);
	}
	{	// Main menu setup
		MainMenu *mainmenu = new MainMenu();
		mainmenu->setBackground(mEngine->provideTextureManager()->create("menu\\menu_background.tga"));
		mainmenu->setFont(mEngine->provideFont());
		mainmenu->init();

		mScreenManager->add(mainmenu);
		mScreenManager->show("screen.mainmenu");
	}

	mEngine->setViewer(mCamera);
	mEngine->reset();


	loadConfig();

	mEngine->aeErrorCheck(__FILE__, __LINE__);

	// load demo scene
	if ( cmdline.length() > 0 ) {
		mEngine->loadScene(cmdline);
	}
	else {
		mEngine->loadScene("flat");
	}
}

void ISCViewer::loadConfig()
{
	if ( !mParser->load(mMediaPathManager->getGame() + "game.conf") ) {
		error("could not load engine config!");
		return;
	}

	// Console settings
	if ( mParser->getValue("console_enabled").toBool() ) {
		mConsole->enable();
	}
}

bool ISCViewer::processKeyPress(WPARAM keyCode)
{
	Input::Key *key = mKeyboard->getKey(keyCode);
	if ( !key ) {
		// an unbund key has been pressed

		switch ( keyCode ) {
			case VK_F1:
				break;
			case VK_F2:
				break;
			case VK_F3:
				break;
			case VK_F4:
				break;
			case VK_RETURN:
				break;
			case 'C': {
					static CameraSpace::IViewer *viewer = mCamera;

					if ( viewer == mCamera ) { viewer = mMovieCamera; }
					else { viewer = mCamera; }

					mEngine->setViewer(viewer);
				}
				break;
			case 'M':
				mCamera->setFollowMouseView(!mCamera->isFollowMouseView());
				break;
		}

		return true;
	}

	if ( key->isFunction("escape") ) {
		if ( mScreenManager->get("screen.engine")->isVisible() ) {
			mCamera->setFollowMouseView(false);
			mEngine->Pause();
			mScreenManager->hide("screen.engine");
		}
		else {
			mScreenManager->show("screen.engine");
		}
	}
	else if ( key->isFunction("play") ) {
		if ( mEngine->isRunning() ) {
			mEngine->Pause();
		}
		else {
			mEngine->Play();
		}
	}

	return true;
}

void ISCViewer::run()
{
}
