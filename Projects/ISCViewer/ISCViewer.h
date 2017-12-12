
#ifndef _ISCViewer_h_
#define _ISCViewer_h_


// Library includes

// Project includes
#include <Application/Application.h>

// Forward declarations
namespace CameraSpace {
	class MovieCamera;
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
class vector2f;
namespace WarehouseSpace {
	class Warehouse;
}

// Application forward declarations
class EditorCamera;

// Namespace declarations
using CameraSpace::MovieCamera;
using Core::IGeneralManager;
using Core::IScreenWriter;
using Core::RenderObject;


class ISCViewer : public Core::Application
{
public:
	ISCViewer(HINSTANCE instance);
	~ISCViewer();

public:
	// Application infrastructure
	// {
	LRESULT handleEvent(const Common::Event &e);

	void initialize(const std::string& cmdLine);
	void run();
	// }

protected:

private:
	// Application infrastructure
	// {
	void loadConfig();
	bool processKeyPress(WPARAM key);
	// }

	// Event handling
	// {
	void dispatchMouseDown(const Common::Event& e);
	void dispatchMouseMove(const Common::Event& e);
	void dispatchMouseUp(const Common::Event& e);
	void dispatchMouseWheel(const Common::Event& e);
	// }

private:
	// Owned objects
	WarehouseSpace::Warehouse	*mWarehouse;

	// Used objects
	EditorCamera			*mCamera;
	MovieCamera				*mMovieCamera;
	Mesh::IObjectManager	*mObjectManager;
	ConfigParser			*mParser;
	Physics::IManager		*mPhysicManager;
	Scene::IManager			*mSceneManager;
	ScriptSpace::IManager	*mScriptManager;
	Terrain::IManager		*mTerrainManager;
};


#endif
