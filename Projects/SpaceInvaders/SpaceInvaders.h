
#ifndef SpaceInvaders_SpaceInvaders_h
#define SpaceInvaders_SpaceInvaders_h


// Library includes

// Project includes
#include <Application/Application.h>
#include <NPC/NPC.h>

// Application includes

// Forward declarations
// Engine classes
// {
namespace CameraSpace {
	class Viewer;
}
class ConfigParser;
namespace Core {
	class RenderObject;
}
class EditorCamera;
namespace Mesh {
	class IObjectManager;
}
namespace Physics {
	class IManager;
}
namespace TextureSpace {
	class IManager;
}
// }
// Game classes
// {
class BulletManager;
namespace Collision {
	class Manager;
}
class Game;
namespace NPCSpace {
	class Manager;
	class Player;
}
// }

// Namespace declarations
using CameraSpace::Viewer;


class SpaceInvaders : public Core::Application
{
public:
	SpaceInvaders(HINSTANCE instance);
	~SpaceInvaders();

public:
	// Application infrastructure
	// {
	LRESULT handleEvent(const Common::Event &e);

	void initialize(const std::string& cmdLine);
	void run();
	// }

protected:

private:
	class State {
	public:
		enum E {
			NotYetStarted,
			IntroBegin,
			IntroEnd,
			Game,
			OutroBegin,
			OutroEnd
		};
	};

private:
	// Application infrastructure
	// {
	void keyDown(WPARAM keyCode);
	void keyUp(WPARAM keyCode);
	void loadConfig();
	void processCollisions();
	void processControls();
	bool processKeyPress(WPARAM keyCode);
	// }

	void advanceToNextLevel();
	void dropPowerUp();
	void gameOver();
	void newGame();
	void nextLevel();
	bool player_hit();
	void player_move(int speed);
	void player_strafe(int speed);
	void shoot();

	void introBegin();
	void introEnd();
	void outroBegin();
	void outroEnd();

private:
	// Engine objects
	EditorCamera			*mCamera;
	Mesh::IObjectManager	*mObjectManager;
	ConfigParser			*mParser;
	Physics::IManager		*mPhysicsManager;
	TextureSpace::IManager	*mTextureManager;
	Viewer					*mViewer;

private:
	BulletManager		*mBulletManager;
	Collision::Manager	*mCollisionManager;
	NPCSpace::Manager	*mNPCManager;

	Game				*mGame;
	NPCSpace::Player	*mPlayer;

private:	// Events
	State::E	mState;
};


#endif
