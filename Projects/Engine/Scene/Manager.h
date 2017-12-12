
#ifndef Scene_Manager_h
#define Scene_Manager_h


// Library includes
#include <list>
#include <vector>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Scene/IManager.h>
#include "Settings.h"

// Forward declarations
namespace CameraSpace {
	class IViewer;
}
class ConfigParser;
namespace Console {
	class IConsoleInterface;
}
namespace Core {
	class EngineSettings;
	class ICoreUtilities;
	class IdGenerator;
}
class Frustum;
class IDriver;
class IMediaPathManager;
namespace Mesh {
	class IObjectManager;
}
namespace Terrain {
	class IManager;
}

// Namespace declarations
using Console::IConsoleInterface;


namespace Scene {


// Forward declarations
class Scene;
class SceneGraphObject;
class SceneObjectParser;


class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

public:
	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);
	//}

	// IManager implementation
	//{
	void attach(SceneGraphObject *child, Core::AEid parent, bool keepPosition = true);
	void attach(SceneGraphObject *child, SceneGraphObject *parent, bool keepPosition = true);
	SceneGraphObject* collidePoint(const vector3f& vector, bool child = false);
	SceneGraphObject* create(Core::RenderObject *ro);
	void detach(Core::AEid id);
	void detach(SceneGraphObject *sgo);
	void erase(SceneGraphObject *sgo);
	SceneGraphObject* getById(Core::AEid id);
	SceneGraphObject* getByName(const std::string& name);
	Scene* getScene();
	bool load(const std::string& filename);
	SceneGraphObject* replicate(SceneGraphObject *sgo);
	//}

	// Connectors
	//{
	void connectConsole(IConsoleInterface *c);
	void connectCoreUtilities(Core::ICoreUtilities *u);
	void connectDriver(IDriver *d);
	void connectEngineSettings(Core::EngineSettings *s);
	void connectFrustum(Frustum *f);
	void connectIdGenerator(Core::IdGenerator *g);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectObjectManager(Mesh::IObjectManager *m);
	void connectTerrainManager(Terrain::IManager *m);
	//}

	// Providers
	//{
	Settings* provideSettings();
	//}

	void render();
	void setViewer(CameraSpace::IViewer *v);
	void updateOcclusionQuerys();

protected:

private:
	typedef std::list<SceneGraphObject*> SceneGraphObjects;

private:
	bool add(const std::string& entity);
	SceneGraphObject* find(Core::AEid id);

private:
	SceneGraphObjects	mSceneGraphObjects;
	Settings			mSettings;

	IConsoleInterface		*mConsole;
	Core::ICoreUtilities	*mCoreUtilities;
	IDriver					*mDriver;
	Core::EngineSettings	*mEngineSettings;
	Frustum					*mFrustum;
	Core::IdGenerator		*mGenerator;
	IMediaPathManager		*mMediaPathManager;
	Mesh::IObjectManager	*mObjectManager;
	Scene					*mScene;
	SceneObjectParser		*mSceneObjectParser;
	Terrain::IManager		*mTerrainManager;
	CameraSpace::IViewer	*mViewer;

	float	mLastOcclusionQuery;
};


}

#endif
