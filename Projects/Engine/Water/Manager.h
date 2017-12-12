
#ifndef _Water_Manager_h_
#define _Water_Manager_h_


// Library includes

// Project includes
#include "Settings.h"
#include <Common/Logger.h>
#include <Interfaces/Water/IManager.h>

// Forward declarations
namespace Console {
	class IConsoleInterface;
}
class IMediaPathManager;
namespace MaterialSpace {
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


namespace WaterSpace {

// Forward declarations
class Water;

class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

public:
	// Common::AManager implementation
	// {
	void configurationComplete();
	void clear();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime = 0.f);
	// }

	// IManager implementation
	// {
	// }

	// Connectors
	// {
	void connectConsole(IConsoleInterface *c);
	void connectMaterialManager(MaterialSpace::IManager *m);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectSceneManager(Scene::IManager *m);
	void connectShaderManager(ShaderSpace::IManager *m);
	void connectTextureManager(TextureSpace::IManager *m);
	// }

	// Providers
	// {
	Settings* provideSettings();
	// }

	void render();

protected:

private:
	typedef std::list<Water*>	WaterList;

private:
	Settings	mSettings;
	WaterList	mWater;

	IConsoleInterface		*mConsole;
	MaterialSpace::IManager	*mMaterialManager;
	IMediaPathManager		*mMediaPathManager;
	Scene::IManager			*mSceneManager;
	ShaderSpace::IManager	*mShaderManager;
	TextureSpace::IManager	*mTextureManager;
};


}


#endif
