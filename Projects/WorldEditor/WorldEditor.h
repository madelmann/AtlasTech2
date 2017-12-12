
#ifndef _WorldEditor_WorldEditor_h_
#define _WorldEditor_WorldEditor_h_


// Library includes

// Project includes
#include <Application/Application.h>
#include <Common/EventQueue.h>

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
namespace Tools {
	namespace Files {
		class FileBrowser;
	}
}
class vector2f;

// Application forward declarations
class EditorCamera;

// Namespace declarations
using CameraSpace::MovieCamera;
using Core::IGeneralManager;
using Core::IScreenWriter;
using Core::RenderObject;


class WorldEditor : public Core::Application
{
public:
	WorldEditor(HINSTANCE instance);
	~WorldEditor();

public:
	LRESULT handleEvent(const Common::Event &e);

	void initialize(const std::string& cmdLine);
	void run();

protected:

private:
	typedef enum Mode_s {
		ModeSelection	= 0,
		ModeInsertion	= 1
	} eMode;

	void dispatchMouseDown(const Common::Event& e);
	void dispatchMouseMove(const Common::Event& e);
	void dispatchMouseUp(const Common::Event& e);
	void dispatchMouseWheel(const Common::Event& e);
	void dropSelection();
	void loadConfig();
	bool onKeyPress(WPARAM key);
	void pickSelection(const vector3f& vector);
	void setEditorMode(eMode mode);
	void shootBox();
	void switchEditorMode();

	vector3f getRayTo(vector2l pos);

	// Events
	// {
	void processDelayedEvents();
	void queue(const Common::Event& e);

	Common::EventQueue	mEventQueue;
	// }

private:
	eMode				mEditorMode;
	std::string			mInsertObject;
	Scene::SceneGraphObject	*mSceneSelection;

	// Owned classes
	Tools::Files::FileBrowser *mFileBrowser;

	// Used classes
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
