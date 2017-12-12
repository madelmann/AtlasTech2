
#ifndef _Clouds_Manager_h_
#define _Clouds_Manager_h_


// Library includes
#include <list>

// Project includes
#include "Settings.h"
#include <Common/Logger.h>
#include <Interfaces/Clouds/IManager.h>

// Forward declarations
namespace Console {
	class IConsoleInterface;
}
namespace Core {
	class EngineStates;
	class RenderObject;
}
class IMediaPathManager;
namespace MaterialSpace {
	class IManager;
}
namespace Mesh {
	class IObjectManager;
}
namespace Scene {
	class IManager;
	class SceneGraphObject;
}
namespace TextureSpace {
	class IManager;
}

// Namespace declarations
using Console::IConsoleInterface;
using Core::EngineStates;


namespace CloudSpace {


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
	// {
	// }

	// Connectors
	//{
	void connectConsole(IConsoleInterface *c);
	void connectEngineStates(EngineStates *s);
	void connectMaterialManager(MaterialSpace::IManager *m);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectMeshManager(Mesh::IObjectManager *m);
	void connectSceneManager(Scene::IManager *m);
	void connectTextureManager(TextureSpace::IManager *m);
	//}

	// Providers
	// {
	Settings* provideSettings();
	// }

protected:

private:
	typedef std::list<Core::RenderObject*> CloudList;
	typedef std::list<Scene::SceneGraphObject*> SceneCloudList;

private:
	CloudList		mClouds;
	SceneCloudList	mSceneClouds;
	Settings		mSettings;

	IConsoleInterface		*mConsole;
	EngineStates			*mEngineStates;
	MaterialSpace::IManager	*mMaterialManager;
	IMediaPathManager		*mMediaPathManager;
	Mesh::IObjectManager	*mMeshManager;
	Scene::IManager			*mSceneManager;
	TextureSpace::IManager	*mTextureManager;
};


}


#endif
