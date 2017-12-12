
#ifndef Arena_Arena_h
#define Arena_Arena_h


// Library includes

// Project includes
#include <Application/Application.h>
#include <ClientSidePrediction/ClientSidePrediction.h>
#include <Common/EventQueue.h>
#include <Interfaces/Renderer/IRendererPlugin.h>
#include <Network/Client/TCPClient.h>
#include <Network/Server/TCPServer.h>
#include <Network/ILocalPlayerManager.h>

// Forward declarations
namespace CameraSpace {
	class Viewer;
}
class ConfigParser;
namespace Core {
	class RenderObject;
}
namespace Mesh {
	class IObjectManager;
}
namespace Physics {
	class IManager;
}
namespace Scene {
	class IManager;
	class SceneGraphObject;
}
namespace ScriptSpace {
	class IManager;
}
namespace Terrain {
	class IManager;
}
namespace TextureSpace {
	class IManager;
}
namespace Tools {
	namespace Files {
		class FileBrowser;
	}
}

// Application forward declarations
class BulletManager;
namespace Collision {
	class Manager;
}
class Editor;
class EditorCamera;
class EntityManager;
namespace ApplicationSpace {
	namespace Terrain {
		class Manager;
	}
}
class Player;
namespace WeaponSpace {
	class Manager;
}

// Namespace declarations
using Core::IGeneralManager;
using Core::IScreenWriter;
using Core::RenderObject;


class Arena : public Core::Application,
			  public Core::IRendererPlugin,
			  public Network::ILocalPlayerManager
{
public:
	Arena(HINSTANCE instance);
	~Arena();

public:
	LRESULT handleEvent(const Common::Event &e);

	void initialize(const std::string& cmdLine);
	void run();

public: // Network::ILocalPlayerManager implementation
	void initLocalPlayer(Core::AEid id, const vector3f& position);

protected:

private:
	void dispatchMouseDown(const Common::Event& e);
	void dispatchMouseUp(const Common::Event& e);

	bool onKeyPress(WPARAM key);

private:
	void loadConfig();
	void render();
	void shoot();
	void shootBox();
	void update(float elapsedTime);

private:	// Owned classes
	BulletManager *mBulletManager;
	Collision::Manager *mCollisionManager;
	Prediction::ClientSidePrediction mClientSidePrediction;
	Editor *mEditor;
	EntityManager *mEntityManager;
	CameraSpace::Viewer *mExternalCamera;
	EditorCamera *mFPSCamera;
	Player *mLocalPlayer;
	Network::Client::TCPClient *mTCPClient;
	Network::Server::TCPServer *mTCPServer;
	ApplicationSpace::Terrain::Manager *mTerrainManager;
	WeaponSpace::Manager *mWeaponManager;

private:	// Used classes
	Mesh::IObjectManager *mObjectManager;
	Physics::IManager *mPhysicManager;
	Scene::IManager *mSceneManager;
	TextureSpace::IManager *mTextureManager;
};


#endif
