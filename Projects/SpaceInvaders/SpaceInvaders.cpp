
// Header
#include "SpaceInvaders.h"

// Library includes

// Project includes
#include <Driver/DriverUtils.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Physics/IManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Interfaces/UIKit/IManager.h>
#include <Misc/Colors.h>
#include <ObjectiveScript/ObjectiveScript.h>
#include <Parsers/ConfigParser.h>
#include <Renderer/RenderObject.h>
#include <Scene/SceneGraphObject.h>
#include <Sound/Sound.h>
#include <Tools/Tools.h>

// Application includes
#include <Bullet/BulletManager.h>
#include <Camera/EditorCamera.h>
#include <Collision/Manager.h>
#include <Game/Game.h>
#include <NPC/Enemy.h>
#include <NPC/Manager.h>
#include <NPC/Player.h>
#include <Screens/Credits.h>
#include <Screens/MainMenu.h>
#include "Types.h"

// Namespace declarations
using SoundSpace::Sound;


SpaceInvaders::SpaceInvaders(HINSTANCE instance)
: Application("SpaceInvaders", instance),
  mBulletManager(0),
  mCamera(0),
  mCollisionManager(0),
  mGame(0),
  mNPCManager(0),
  mObjectManager(0),
  mParser(0),
  mPlayer(0),
  mPhysicsManager(0),
  mState(State::NotYetStarted),
  mTextureManager(0)
{
	debug("Starting...");
}

SpaceInvaders::~SpaceInvaders()
{
	debug("Stopping...");

	aeDelete( mBulletManager );
	aeDelete( mCamera );
	aeDelete( mCollisionManager );
	aeDelete( mGame );
	aeDelete( mNPCManager );
	aeDelete( mParser );
	aeDelete( mPlayer );
}

void SpaceInvaders::advanceToNextLevel()
{
	mGame->stop();

	mGame->nextLevel();

	debug("ADVANCE TO NEXT LEVEL: e" + Tools::toString(mGame->getAct()) + "m" + Tools::toString(mGame->getLevel()));

	nextLevel();

	mGame->start();
}

void SpaceInvaders::dropPowerUp()
{
	debug("DROP POWERUP");
}

void SpaceInvaders::gameOver()
{
	debug("GAME OVER");
}

LRESULT SpaceInvaders::handleEvent(const Common::Event &e)
{
	switch ( e.uMsg ) {
		case WM_KEYDOWN:
			processKeyPress(e.wParam);
  			keyDown(e.wParam);
			break;
		case WM_KEYUP:
			keyUp(e.wParam);
			break;
	}

	return true;
}

void SpaceInvaders::initialize(const std::string& /*cmdLine*/)
{
	debug("Initializing...");

	Application::initialize();
	Application::processAdd(this);


	mBulletManager = new BulletManager(this);
	mCamera = new EditorCamera();
	mCollisionManager = new Collision::Manager(this);
	mGame = new Game();
	mNPCManager = new NPCSpace::Manager(this);
	mObjectManager = mEngine->provideObjectManager();
	mParser = new ConfigParser(this);
	mPhysicsManager = mEngine->providePhysicsManager();
	mTextureManager = mEngine->provideTextureManager();

	mBulletManager->connectCollisionManager(mCollisionManager);
	mBulletManager->connectIdGenerator(mEngine->provideIdGenerator());
	mBulletManager->connectObjectManager(mObjectManager);
	mBulletManager->connectSceneManager(mEngine->provideSceneManager());
	mBulletManager->connectTextureManager(mEngine->provideTextureManager());

	mCamera->connectEngine(mEngine);

	mNPCManager->connectCollisionManager(mCollisionManager);
	mNPCManager->connectMediaPathManager(mEngine->provideMediaPathManager());
	mNPCManager->connectObjectManager(mObjectManager);
	mNPCManager->connectSceneManager(mSceneManager);

	mBulletManager->configurationComplete();
	mCamera->configurationComplete();
	mCollisionManager->configurationComplete();
	mNPCManager->configurationComplete();


	mGeneralManager->add(mBulletManager);
	mGeneralManager->add(mCollisionManager);
	mGeneralManager->add(mNPCManager);


	{	// Credits screen setup
		ScreenCredits *credits = new ScreenCredits();
		credits->init();

		mScreenManager->add(credits);
	}
	{	// Main menu setup
		MainMenu *mainmenu = new MainMenu();
		mainmenu->connectTextureManager(mTextureManager);
		mainmenu->setBackground(mEngine->provideTextureManager()->create("menu/menu_background.jpg"));
		mainmenu->setFont(mEngine->provideFont());
		mainmenu->init();

		mScreenManager->add(mainmenu);
	}
	mScreenManager->show("screen.mainmenu");

	mEngine->setViewer(mCamera);

	mKeyboard->addKey(VK_DOWN, "down");
	mKeyboard->addKey(VK_LEFT, "left");
	mKeyboard->addKey(VK_RIGHT, "right");
	mKeyboard->addKey(VK_SPACE, "shoot");
	mKeyboard->addKey(VK_UP, "up");
	mKeyboard->addKey('T', "teleport");

	loadConfig();

	mPlayer = new NPCSpace::Player();
}

void SpaceInvaders::introBegin()
{
	mState = State::IntroBegin;

	mEngineSettings->useMotionBlur(true);
	mEngineSettings->setMotionBlurValue(0.75f);
}

void SpaceInvaders::introEnd()
{
	mState = State::IntroEnd;

	mEngineSettings->useMotionBlur(false);
}

void SpaceInvaders::keyDown(WPARAM keyCode)
{
	Input::Key *key = mKeyboard->getKey(keyCode);
	if ( !key ) {
		// an unbund key has been pressed => ignore it and exit
		return;
	}

	key->setState(Input::Key::State::Down);
}

void SpaceInvaders::keyUp(WPARAM keyCode)
{
	Input::Key *key = mKeyboard->getKey(keyCode);
	if ( !key ) {
		// an unbund key has been released => ignore it and exit
		return;
	}

	key->setState(Input::Key::State::Up);
}

void SpaceInvaders::loadConfig()
{
	if ( !mParser->load(mMediaPathManager->getGame() + "game.conf") ) {
		error("could not load config!");
		return;
	}

	// Console settings
	if ( mParser->getValue("console_enabled").toBool() ) {
		mConsole->enable();
	}
}

void SpaceInvaders::newGame()
{
	mGame->newGame();
	nextLevel();

	mGame->start();
}

void SpaceInvaders::nextLevel()
{
	// load scene
	std::string scene = "e" + Tools::toString(mGame->getAct()) + "m" + Tools::toString(mGame->getLevel());
	bool result = mEngine->loadScene(scene);
	if ( !result ) {
		error("could not load scene '" + scene + "'!");
	}

	// Player setup
	// {
	Core::RenderObject *player = mObjectManager->create("player");

	Scene::SceneGraphObject *player_sgo = mSceneManager->create(player);
	player_sgo->collisionIdentifier(PlayerShip);
	mSceneManager->attach(player_sgo, Core::invalidID, true);

	mCollisionManager->registerSGO(player_sgo);

	mPlayer->health((float)mGame->getLifes());
	mPlayer->setSceneGraphObject(player_sgo);
	mPlayer->init(vector3f(0.f, 0.f, -96.f), vector3f(0.f, 0.f, 0.f));
	// }

	mScreenManager->show("screen.engine");

	mEngine->Play();
}

void SpaceInvaders::outroBegin()
{	
	mState = State::OutroBegin;

	mEngineSettings->useMotionBlur(true);
	mEngineSettings->setMotionBlurValue(0.9f);
}

void SpaceInvaders::outroEnd()
{
	mState = State::OutroEnd;

	mEngineSettings->useMotionBlur(false);
}

bool SpaceInvaders::player_hit()
{
	// check if the player is invincible

	// the player has been killed
	// as long as he has "coins" left, he will get respawned

	debug("RESPAWN");

	mPlayer->health(mPlayer->health() - 1);

	mPlayer->init(vector3f(0.f, 0.f, -96.f), vector3f(0.f, 0.f, 0.f));

	if ( mPlayer->health() <= 0 ) {
		gameOver();
		return false;
	}

	return true;
}

void SpaceInvaders::player_move(int speed)
{
	float frameTime = mEngine->getFrameTime();
	vector3f pos = mPlayer->position();
	vector3f rot = mPlayer->rotation();

	pos.z += speed * frameTime;
	//rot += vector3f(0.f, 0.f, -4.f * speed * frameTime);

	if ( pos.z <= -96.f ) {
		pos.z = -96.f;
		rot.x = 0.f;
	}
	if ( pos.z >= 96.f ) {
		pos.z = 96.f;
		rot.x = 0.f;
	}

	mPlayer->position(pos);
	mPlayer->rotation(rot);
}

void SpaceInvaders::player_strafe(int speed)
{
	float frameTime = mEngine->getFrameTime();
	vector3f pos = mPlayer->position();
	vector3f rot = mPlayer->rotation();

	pos.x += speed * frameTime;
	//rot += vector3f(0.f, 0.f, -4.f * speed * frameTime);

	if ( pos.x <= -128.f ) {
		pos.x = -128.f;
		rot.z = 0.f;
	}
	if ( pos.x >= 128.f ) {
		pos.x = 128.f;
		rot.z = 0.f;
	}

	mPlayer->position(pos);
	mPlayer->rotation(rot);
}

void SpaceInvaders::processCollisions()
{
	Physics::CollisionGroupsList groups = mCollisionManager->getCollisionGroups();
	for ( Physics::CollisionGroupsList::iterator gIt = groups.begin(); gIt != groups.end(); ++gIt ) {
		Core::IdSet colliders = (*gIt).getColliders(Core::invalidID);

		typedef std::list<Scene::SceneGraphObject*> SGOList;
		SGOList sgoList;
		for ( Core::IdSet::const_iterator idIt = colliders.begin(); idIt != colliders.end(); ++idIt ) {
			// get Scene::SceneGraphObject from Core::Id ...
			Scene::SceneGraphObject *sgo = mSceneManager->getById((*idIt));
			// ... and add it to our list
			if ( sgo ) {
				sgoList.push_back(sgo);
			}
		}

		for ( SGOList::iterator sgoIt = sgoList.begin(); sgoIt != sgoList.end(); ++sgoIt ) {
			// check if the player has been hit
			if ( mPlayer->getSceneGraphObject() == (*sgoIt) ) {
				if ( player_hit() ) {
					continue;
				}
			}

			if ( mNPCManager->hit((*sgoIt)) ) {
				// everything neccessary has been done elsewhere
				continue;
			}

			// here would also be space for a boss opponent...

			mBulletManager->removeReference((*sgoIt));
			mCollisionManager->unregisterSGO((*sgoIt));
			mSceneManager->erase((*sgoIt));
			(*sgoIt) = 0;
		}

		sgoList.clear();
	}
}

void SpaceInvaders::processControls()
{
	int speed = 32;

	// controls
	if ( mKeyboard->getKey("down")->getState() == Input::Key::State::Down ) {
		player_move(-speed / 2);
	}
	if ( mKeyboard->getKey("left")->getState() == Input::Key::State::Down ) {
		player_strafe(speed);
	}
	if ( mKeyboard->getKey("right")->getState() == Input::Key::State::Down ) {
		player_strafe(-speed);
	}
	if ( mKeyboard->getKey("up")->getState() == Input::Key::State::Down ) {
		player_move(speed / 2);
	}

	// additional
	if ( mKeyboard->getKey("shoot")->getState() == Input::Key::State::Down ) {
		shoot();
	}
}

bool SpaceInvaders::processKeyPress(WPARAM keyCode)
{
	Input::Key *key = mKeyboard->getKey(keyCode);
	if ( !key ) {
		// an unbund key has been pressed => ignore it and exit
		return false;
	}

	if ( key->isFunction("escape") ) {
		if ( mScreenManager->get("screen.engine")->isVisible() ) {
			mCamera->setFollowMouseView(false);
			mEngine->Pause();
			mScreenManager->hide("screen.engine");
		}
	}
	else if ( key->isFunction("freecamera") ) {
		mCamera->setFollowMouseView(!mCamera->isFollowMouseView());
	}
	else if ( key->isFunction("play") ) {
		if ( mEngine->isRunning() ) { mEngine->Pause(); }
		else { mEngine->Play(); }
	}
	else if ( key->isFunction("return") ) {
		if ( !mGame->isStarted() ) {
			newGame();
		}
	}
	else if ( key->isFunction("teleport") ) {
		mPlayer->init(vector3f(0.f, 0.f, -96.f), vector3f(0.f, 0.f, 0.f));
	}

	return true;
}

void SpaceInvaders::run()
{
	{	// Game interface
		mScreenWriter->write(-0.97f, -0.93f, "Coins: " + Tools::toString(mPlayer->health()), 0, Color::turquoise);
	}

	if ( mGame->isStarted() ) {	// Game logic
		processCollisions();
		processControls();

		if ( !mNPCManager->isThereAnEnemyStillAlive() ) {
			advanceToNextLevel();
		}
	}
}

void SpaceInvaders::shoot()
{
	static float lastShot = 0.f;
	float now = mEngine->getRunTime();

	if ( now - lastShot < 0.5f ) {
		return;
	}

	mBulletManager->add(vector3f(mPlayer->position()) + vector3f(-2.0f, -0.1f, 28.f), mPlayer->rotation());

	//Sound *s = mSoundManager->create2D("roll.wav");
	//s->play();

	lastShot = now;
}
