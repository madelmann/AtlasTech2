
// Header
#include "TestDrive.h"

// Library includes

// Project includes
#include <Camera\Viewer.h>
#include <Driver\DriverUtils.h>
#include <Interfaces\Core\Types.h>
#include <interfaces\Media\IMediaPathManager.h>
#include <Interfaces\Mesh\IObjectManager.h>
#include <Interfaces\Scene\IManager.h>
#include <Interfaces\Terrain\IManager.h>
#include <Interfaces\Texture\IManager.h>
#include <Interfaces\UIKit\IManager.h>
#include <Math\Normals.h>
#include <Math\Vector2.h>
#include <ObjectiveScript/ObjectiveScript.h>
#include <Parsers\ConfigParser.h>
#include <Renderer\RenderObject.h>
#include <Scene\SceneGraphObject.h>
#include <Tools\Tools.h>

// Application includes
#include <Camera\External.h>
#include <Menu\MainMenu.h>
#include <Player\Player.h>
#include <Vehicle\Vehicle.h>
#include <Vehicle\VehicleManager.h>

// Namespace declarations


TestDrive::TestDrive(HINSTANCE instance)
: Application("TestDrive", instance),
  mExternalCamera(0),
  mFPSCamera(0),
  mGameStarted(false),
  mObjectManager(0),
  mParser(0),
  mVehicleManager(0)
{
	debug("Starting...");
}

TestDrive::~TestDrive()
{
	debug("Stopping...");

	aeDelete( mExternalCamera );
	aeDelete( mFPSCamera );
	aeDelete( mParser );
	aeDelete( mPlayer );
	aeDelete( mVehicleManager );
}

LRESULT TestDrive::handleEvent(const Common::Event &e)
{
	switch ( e.uMsg ) {
		case WM_KEYDOWN:
			processKeyPress(e.wParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			break;
	}

	return true;
}

void TestDrive::initialize(const std::string& /*cmdLine*/)
{
	debug("Initializing...");

	Application::initialize();
	Application::processAdd(this);


	mExternalCamera = new CameraSpace::Viewer();
	mFPSCamera = new External();
	mGeneralManager = mEngine->provideGeneralManager();
	mObjectManager = mEngine->provideObjectManager();
	mParser = new ConfigParser(this);
	mSceneManager = mEngine->provideSceneManager();
	mTerrainManager = mEngine->provideTerrainManager();
	mVehicleManager = new VehicleManager(this);


	mVehicleManager->connectMediaPathManager(mEngine->provideMediaPathManager());
	mVehicleManager->connectObjectManager(mEngine->provideObjectManager());
	mVehicleManager->connectSceneManager(mEngine->provideSceneManager());
	mVehicleManager->connectTerrainManager(mEngine->provideTerrainManager());

	mVehicleManager->configurationComplete();

	mVehicleManager->init();

	mGeneralManager->add(mVehicleManager);


	{
		MainMenu *mainmenu = new MainMenu();
		mainmenu->setBackground(mEngine->provideTextureManager()->create("menu\\menu_background.tga"));
		mainmenu->setFont(mEngine->provideFont());
		mainmenu->init();

		mScreenManager->add(mainmenu);
		mScreenManager->show("screen.mainmenu");
	}


	mEngine->setViewer(mExternalCamera);

	loadConfig();

	mEngine->aeErrorCheck(__FILE__, __LINE__);

	mPlayer = new Player();
}

void TestDrive::loadConfig()
{
	if ( !mParser->load(mMediaPathManager->getGame() + "game.conf") ) {
		error("could not load game config!");
		return;
	}

	// Console settings
	if ( mParser->getValue("console_enabled").toBool() ) {
		mConsole->enable();
	}
}

void TestDrive::newGame()
{
	mEngine->reset();
	mEngine->loadScene("nvidia");

	mGameStarted = true;

	// Initialize settings
	mExternalCamera->position(vector3f(2048.f, 512.f, 2048.f));
	mExternalCamera->view(vector3f(2048.f, 0.f, 2048.f));
	mExternalCamera->upVector(vector3f(0.f, 1.f, 0.f));

	mFPSCamera->upVector(vector3f(0.f, 1.f, 0.f));


	Vehicle *vehicle = mVehicleManager->create("test.vehicle");
	vehicle->sceneGraphObject()->init(vector3f(512.f, 0.f, 512.f), vector3f(0.f, 0.f, 0.f));

	mPlayer->setVehicle(vehicle);
	mPlayer->setPosition(vector3f(512.f, 0.f, 512.f));
	mPlayer->setRotation(vector3f(0.f, 0.f, 0.f));

	mFPSCamera->offset(vehicle->cameraOffset());

	mScreenManager->show("screen.engine");
	mEngine->Play();
}

bool TestDrive::processKeyPress(WPARAM key)
{
	switch ( key ) {
		case VK_ESCAPE:
			if ( mScreenManager->get("screen.engine")->isVisible() ) {
				mEngine->Pause();
				mScreenManager->hide("screen.engine");
			}
			break;
		case 'E':
			mEngine->setViewer(mExternalCamera);
			break;
		case 'F':
			mEngine->setViewer(mFPSCamera);
			break;
		case VK_RETURN:
			newGame();
			break;
	}

	return true;
}

void TestDrive::run()
{
	static vector3f color(0.f, 0.f, 1.f);
	char text[255] = "";

	if ( mGameStarted ) {
		mPlayer->checkMovement();

		vector3f vec;

		vec = mPlayer->getPosition();
		sprintf(text, "X = %.2f, Y = %.2f, Z = %.2f", vec.x, vec.y, vec.z);
		mScreenWriter->write(-0.97f, 0.79f, text, 0, color);

		vec = mPlayer->getRotation();
		sprintf(text, "X = %.2f, Y = %.2f, Z = %.2f", vec.x, vec.y, vec.z);
		mScreenWriter->write(-0.97f, 0.88f, text, 0, color);

		// External Camera
		{
			mExternalCamera->view(mPlayer->getPosition());

			vec = vector3f(2048.f, 1024.f, 2048.f);
			vec -= mPlayer->getPosition();
			vec = normalize(vec);
			vec = vec * 256.f + mPlayer->getPosition();
			vec.y = mPlayer->getPosition().y + 256.f;

			vector3f terrainHeight = mTerrainManager->heightAt(vec);
			if ( vec.y - terrainHeight.y < 128.f ) {
				vec.y = terrainHeight.y + 128.f;
			}

			mExternalCamera->position(vec);
		}
		// First person camera
		{
			mFPSCamera->position(mPlayer->cameraOffset() + mPlayer->getPosition());
			mFPSCamera->view(mPlayer->cameraOffset() + mPlayer->view());
		}
	}
}
