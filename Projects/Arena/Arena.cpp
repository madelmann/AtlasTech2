
// Header
#include "Arena.h"

// Library includes

// Project includes
#include <Camera/MovieCamera/MovieCamera.h>
#include <Camera/Viewer.h>
#include <Driver/DriverUtils.h>
#include <Interfaces/Console/IConsole.h>
#include <Interfaces/Core/Types.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Physics/IManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Script/IManager.h>
#include <Interfaces/Terrain/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Interfaces/UIKit/IManager.h>
#include <Math/Tools.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <ObjectiveScript/ObjectiveScript.h>
#include <Parsers/ConfigParser.h>
#include <Renderer/RenderObject.h>
#include <Scene/SceneGraphObject.h>
#include <Tools/Files.h>

// Application includes
#include <Bullet/BulletManager.h>
#include <Camera/EditorCamera.h>
#include <Collision/Manager.h>
#include <Console/Plugins/ClientPlugin.h>
#include <Console/Plugins/ServerPlugin.h>
#include <Editor/Editor.h>
#include <Entity/EntityManager.h>
#include <Menu/MainMenu.h>
#include <Player/Player.h>
#include <Terrain/Manager.h>
#include <Weapon/Manager.h>

// Namespace declarations


Arena::Arena(HINSTANCE instance)
: Application("Arena", instance),
  IRendererPlugin(Core::invalidID, "Arena"),
  mBulletManager(0),
  mCollisionManager(0),
  mEditor(0),
  mEntityManager(0),
  mExternalCamera(0),
  mFPSCamera(0),
  mLocalPlayer(0),
  mObjectManager(0),
  mPhysicManager(0),
  mSceneManager(0),
  mTCPClient(0),
  mTCPServer(0),
  mTerrainManager(0),
  mWeaponManager(0)
{
	debug("Starting...");
}

Arena::~Arena()
{
	debug("Stopping...");

	mEngine->unregisterPlugin(this);

	aeDelete( mBulletManager );
	aeDelete( mCollisionManager );
	aeDelete( mEditor );
	aeDelete( mEntityManager );
	aeDelete( mExternalCamera );
	aeDelete( mFPSCamera );
	aeDelete( mLocalPlayer );
	aeDelete( mTerrainManager );
	aeDelete( mWeaponManager );

	aeDelete( mTCPClient );
	aeDelete( mTCPServer );
}

void Arena::dispatchMouseDown(const Common::Event& e)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}

	switch ( e.uMsg ) {
		case WM_LBUTTONDOWN:
			if ( mEngine->isRunning() ) {
				shoot();
			}
			break;
		case WM_MBUTTONDOWN:
			break;
		case WM_RBUTTONDOWN:
			break;
	}
}

void Arena::dispatchMouseUp(const Common::Event& /*e*/)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}

/*
	switch ( e.uMsg ) {
		case WM_LBUTTONUP:
			break;
		case WM_MBUTTONUP:
			break;
		case WM_RBUTTONUP:
			break;
	}
*/
}

LRESULT Arena::handleEvent(const Common::Event& e)
{
	switch ( e.uMsg ) {
		case WM_KEYDOWN:
			onKeyPress(e.wParam);
			return true;
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			dispatchMouseDown(e);
			return true;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			dispatchMouseUp(e);
			return true;
	}

	return false;
}

void Arena::initialize(const std::string& cmdline)
{
	debug("Initializing...");

	mBulletManager = new BulletManager(this);
	mCollisionManager = new Collision::Manager(this);
	mEditor = new Editor();
	mEntityManager = new EntityManager();
	mExternalCamera = new CameraSpace::Viewer();
	mFPSCamera = new EditorCamera();
	mTerrainManager = new ApplicationSpace::Terrain::Manager();
	mWeaponManager = new WeaponSpace::Manager(this);

	mEngine->replaceTerrainManager(mTerrainManager);


	Application::initialize();

	mGeneralManager = mEngine->provideGeneralManager();
	mObjectManager = mEngine->provideObjectManager();
	mPhysicManager = mEngine->providePhysicsManager();
	mSceneManager = mEngine->provideSceneManager();
	mTextureManager = mEngine->provideTextureManager();


	mFPSCamera->connectEngine(mEngine);

	{	// Main menu setup
		MainMenu *mainmenu = new MainMenu();
		mainmenu->setBackground(mEngine->provideTextureManager()->create("menu/menu_background.tga"));
		mainmenu->setFont(mEngine->provideFont());
		mainmenu->init();

		mScreenManager->add(mainmenu);
		mScreenManager->show("screen.mainmenu");
	}

	mBulletManager->connectCollisionManager(mCollisionManager);
	mBulletManager->connectIdGenerator(mIdGenerator);
	mBulletManager->connectObjectManager(mObjectManager);
	mBulletManager->connectSceneManager(mSceneManager);

	mEntityManager->connectObjectManager(mObjectManager);
	mEntityManager->connectSceneManager(mSceneManager);

	mTerrainManager->connectTextureManager(mTextureManager);
	mTerrainManager->configurationComplete();

	mWeaponManager->connectObjectManager(mObjectManager);


	mBulletManager->configurationComplete();
	mCollisionManager->configurationComplete();


	mGeneralManager->add(mBulletManager);
	mGeneralManager->add(mCollisionManager);

	mEngine->registerPlugin(this);
	mEngine->setViewer(mFPSCamera);
	mEngine->reset();


	// Networking setup
	{
		mTCPClient = new Network::Client::TCPClient(this);

		mTCPServer = new Network::Server::TCPServer(this);
		mTCPServer->connectEntityManager(mEntityManager);
		mTCPServer->connectIdGenerator(mIdGenerator);
		if ( cmdline.find("--server=yes") != std::string::npos ) {
			mTCPServer->init();
		}

		mTCPClient->connectEntityManager(mEntityManager);
		mTCPClient->connectLocalPlayerManager(this);
		mTCPClient->connectPredictionClient(&mClientSidePrediction);
		if ( cmdline.find("--client=yes") != std::string::npos ) {
			mTCPClient->init();
			//mTCPClient->connect();
		}

		mClientSidePrediction.connectSender(mTCPClient);
	}

	{	// Console plugin setup
		Console::Plugins::ClientPlugin *client = new Console::Plugins::ClientPlugin();
		client->connectClient(mTCPClient);
		mConsole->registerPlugin(client);

		Console::Plugins::ServerPlugin *server = new Console::Plugins::ServerPlugin();
		server->connectServer(mTCPServer);
		mConsole->registerPlugin(server);
	}

	mKeyboard->addKey(69,		"editor");
	mKeyboard->addKey(VK_LEFT,	"editor_left");
	mKeyboard->addKey(VK_RIGHT,	"editor_right");
	mKeyboard->addKey(VK_UP,	"editor_up");
	mKeyboard->addKey(VK_DOWN,	"editor_down");

	mEngine->setViewer(mExternalCamera);

	loadConfig();

	// load demo scene
	mEngine->loadScene("dm1");

	initLocalPlayer(Core::invalidID, vector3f(0.f, 10.f, 0.f));
}

void Arena::initLocalPlayer(Core::AEid id, const vector3f& position)
{
	if ( mLocalPlayer ) {
		aeDelete( mLocalPlayer );
	}

	mClientSidePrediction.setClientId(id);

	// Initialize settings
	mExternalCamera->position(vector3f(1024.f, 2048.f, -512.f));
	mExternalCamera->view(vector3f(1024.f, 0.f, 1024.f));
	mExternalCamera->upVector(vector3f(0.f, 1.f, 0.f));

	mFPSCamera->view(vector3f(0.f, 0.f, 1024.f) + position);
	mFPSCamera->position(position);

	mLocalPlayer = new Player(id);
	mLocalPlayer->mMesh = mSceneManager->create(mObjectManager->create("marine"));
	mSceneManager->attach(mLocalPlayer->mMesh);
	mLocalPlayer->mWeapon = mWeaponManager->create("blaster");


	Scene::SceneGraphObject *sgo = mSceneManager->create(mObjectManager->create("crate"));
	sgo->hide();
	mSceneManager->attach(sgo);

	mEditor->setMesh(sgo);

	mEditor->enabled(true);
}

void Arena::loadConfig()
{
	ConfigParser config(this);

	if ( !config.load(mMediaPathManager->getGame() + "game.conf") ) {
		error("could not load engine configuration!");
		return;
	}

	// Console settings
	if ( config.getValue("console_enabled").toBool() ) {
		mConsole->enable();
	}
}

bool Arena::onKeyPress(WPARAM keyCode)
{
	Input::Key *key = mKeyboard->getKey(keyCode);
	if ( !key ) {
		// an unbund key has been pressed
		switch ( keyCode ) {
			case VK_SPACE:
				shootBox();
				break;
		}

		return true;
	}

	if ( key->isFunction("editor") ) {
		// toggle edit mode on/off
		mEditor->enabled(!mEditor->enabled());

		if ( mEditor->enabled() ) {
			mEngine->setViewer(mExternalCamera);
			mLocalPlayer->mMesh->show();
		}
		else {
			mEngine->setViewer(mFPSCamera);
			//mLocalPlayer->mMesh->hide();
		}
	}
	else if ( key->isFunction("escape") ) {
		if ( mScreenManager->get("screen.engine")->isVisible() ) {
			mFPSCamera->setFollowMouseView(false);
			mEngine->Pause();
			mScreenManager->hide("screen.engine");
		}
		else {
			//mFPSCamera->setFollowMouseView(true);
			mEngine->Play();
			mScreenManager->show("screen.engine");
		}
	}
	else if ( key->isFunction("freecamera") ) {
		mFPSCamera->setFollowMouseView(!mFPSCamera->isFollowMouseView());
	}

	if ( mEditor->enabled() ) {
		Editor::Position pos = mEditor->position();

		if ( key->isFunction("editor_right") ) {
			pos.x -= 1;
			if ( pos.x < 0 ) pos.x = 0;
		}
		else if ( key->isFunction("editor_left") ) {
			pos.x += 1;
			if ( pos.x > 15 ) pos.x = 15;
		}
		else if ( key->isFunction("editor_down") ) {
			pos.y -= 1;
			if ( pos.y < 0 ) pos.y = 0;
		}
		else if ( key->isFunction("editor_up") ) {
			pos.y += 1;
			if ( pos.y > 15 ) pos.y = 15;
		}

		mEditor->position(pos);
	}

	return true;
}

void Arena::render()
{
	if ( mEditor->enabled() ) {
		// editor is enabled => render it according to our raster size
	}
	else {
		if ( mLocalPlayer ) {
			// render player's weapon
			if ( mLocalPlayer->mWeapon ) {
				mLocalPlayer->mWeapon->render();
			}
		}
	}
}

void Arena::run()
{
	mTCPServer->listen();

	mTCPClient->receive();

	if ( !mEditor->enabled() && mEngine->isRunning() ) {
		// Once we have the frame interval, we find the current speed
		float speed = 128.0f * mEngine->getFrameTime();

		// Check if we hit the "run" key
		if ( GetKeyState(VK_SHIFT) & 0x80 ) {
			speed *= 4;
		}

		// Check if we hit the "strafe left" key
		if ( GetKeyState('A') & 0x80 ) {
			mFPSCamera->strafe(-speed);
		}
		// Check if we hit the "strafe right" key
		if ( GetKeyState('D') & 0x80 ) {
			mFPSCamera->strafe(speed);
		}
		// Check if we hit the "backward" key
		if ( GetKeyState('S') & 0x80 ) {
			mFPSCamera->move(-speed);
		}
		// Check if we hit the "forward" key
		if ( GetKeyState('W') & 0x80 ) {
			mFPSCamera->move(speed);
		}

		if ( GetKeyState(VK_DOWN) & 0x80 ) {
			mFPSCamera->move(-speed);
		}
		if ( GetKeyState(VK_LEFT) & 0x80 ) {
			mFPSCamera->rotateY(speed);
		}
		if ( GetKeyState(VK_RIGHT) & 0x80 ) {
			mFPSCamera->rotateY(-speed);
		}
		if ( GetKeyState(VK_UP) & 0x80 ) {
			mFPSCamera->move(speed);
		}

		mLocalPlayer->mMesh->setPosition(mFPSCamera->position());
		mLocalPlayer->mMesh->setRotation(mFPSCamera->rotation());

		mFPSCamera->position(
			mClientSidePrediction.update(mFPSCamera->position(), mFPSCamera->rotation())
		);
	}

	{	// Camera position
		vector3f v = mFPSCamera->position();

		mScreenWriter->write(-0.97f, 0.720f, "Camera.X = " + Tools::toString(v.x));
		mScreenWriter->write(-0.97f, 0.670f, "Camera.Y = " + Tools::toString(v.y));
		mScreenWriter->write(-0.97f, 0.620f, "Camera.Z = " + Tools::toString(v.z));
	}
}

void Arena::shoot()
{
	mBulletManager->add("blaster", mFPSCamera->position(), mFPSCamera->view());
}

void Arena::shootBox()
{
	mPhysicManager->shootBox(mFPSCamera->position(), mFPSCamera->view());
}

void Arena::update(float elapsedTime)
{
	if ( mLocalPlayer && mLocalPlayer->mWeapon ) {
		mLocalPlayer->mWeapon->update(elapsedTime);
	}

	static float elapsed = 0.f;
	elapsed += elapsedTime;

	if ( elapsed > 0.3f ) {
		mTCPClient->keepAlive();
		elapsed = 0.f;
	}
}
