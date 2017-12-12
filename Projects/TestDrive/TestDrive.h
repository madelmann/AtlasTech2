
#ifndef _TestDrive_TestDrive_h_
#define _TestDrive_TestDrive_h_


// Library includes

// Project includes
#include <Application/Application.h>

// Forward declarations
namespace CameraSpace {
	class Viewer;
}
class ConfigParser;
class External;
namespace Mesh {
	class IObjectManager;
}
class Player;
namespace Scene {
	class IManager;
}
namespace Terrain {
	class IManager;
}
class vector2f;

// Application forward declarations
class VehicleManager;

// Namespace declarations
using CameraSpace::Viewer;
using Console::IConsole;
using Core::IScreenWriter;


class TestDrive : public Core::Application
{
public:
	TestDrive(HINSTANCE instance);
	~TestDrive();

public:
	LRESULT handleEvent(const Common::Event &e);

	void initialize(const std::string& cmdLine);
	void run();

protected:

private:
	void loadConfig();
	bool processKeyPress(WPARAM key);

	// Game
	// {
	void newGame();

	bool	mGameStarted;
	// }

private:
	// Used objects
	Viewer					*mExternalCamera;
	External				*mFPSCamera;
	Mesh::IObjectManager	*mObjectManager;
	ConfigParser			*mParser;
	Player					*mPlayer;
	Scene::IManager			*mSceneManager;
	Terrain::IManager		*mTerrainManager;
	VehicleManager			*mVehicleManager;
	Viewer					*mViewer;
};


#endif
