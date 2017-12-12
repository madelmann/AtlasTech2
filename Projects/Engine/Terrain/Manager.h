
#ifndef Terrain_Manager_h
#define Terrain_Manager_h


// Library includes
#include <list>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Terrain/IManager.h>
#include "Settings.h"

// Forward Declarations
namespace Console {
	class IConsoleInterface;
}
namespace Core {
	class EngineSettings;
	class ICoreUtilities;
}
namespace MaterialSpace {
	class IManager;
}
class IMediaPathManager;
namespace Physics {
	class IManager;
}
namespace Scene {
	class IManager;
}
namespace ShaderSpace {
	class IManager;
}
namespace TextureSpace {
	class IManager;
}

// Namespace declarations
using Console::IConsoleInterface;


namespace Terrain {

// Forward declarations
class ALayer;


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
	vector3f heightAt(const vector3f& pos);
	vector3f normalAt(const vector3f& pos);

	void render();
	//}

	// Connectors
	//{
	void connectConsole(IConsoleInterface *c);
	void connectCoreUtilities(Core::ICoreUtilities *u);
	void connectEngineSettings(Core::EngineSettings *s);
	void connectMaterialManager(MaterialSpace::IManager *m);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectPhysicManager(Physics::IManager *m);
	void connectSceneManager(Scene::IManager *m);
	void connectShaderManager(ShaderSpace::IManager *m);
	void connectTextureManager(TextureSpace::IManager *m);
	//}

	// Providers
	// {
	Settings* provideSettings();
	// }

protected:

private:
	typedef std::list<ALayer*> LayerList;

private:
	bool add(const std::string &filename);
	bool load();

private:
	LayerList	mLayers;
	Settings	mSettings;

	IConsoleInterface		*mConsole;
	Core::ICoreUtilities	*mCoreUtilities;
	Core::EngineSettings	*mEngineSettings;
	MaterialSpace::IManager	*mMaterialManager;
	IMediaPathManager		*mMediaPathManager;
	Physics::IManager		*mPhysicManager;
	Scene::IManager			*mSceneManager;
	ShaderSpace::IManager	*mShaderManager;
	TextureSpace::IManager	*mTextureManager;
};


}


#endif
