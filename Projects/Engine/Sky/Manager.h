
#ifndef _Sky_Manager_h_
#define _Sky_Manager_h_


// Library includes

// Project includes
#include "Settings.h"
#include <Common/Logger.h>
#include <Interfaces/Sky/IManager.h>

// Forward declarations
namespace Console {
	class IConsoleInterface;
}
namespace Core {
	class IdGenerator;
	class RenderObject;
}
class IMediaPathManager;
namespace Scene {
	class IManager;
}
namespace TextureSpace {
	class IManager;
}

// Namespace declarations


namespace SkySpace {

// Forward declarations
class ASky;

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

	// Connectors
	//{
	void connectConsole(Console::IConsoleInterface *c);
	void connectIdGenerator(Core::IdGenerator *g);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectSceneManager(Scene::IManager *m);
	void connectTextureManager(TextureSpace::IManager *m);
	//}

	// Providers
	// {
	Settings* provideSettings();
	// }

	void render();

protected:

private:
	Settings	mSettings;
	ASky		*mSky;

	Console::IConsoleInterface	*mConsole;
	Core::IdGenerator			*mIdGenerator;
	IMediaPathManager			*mMediaPathManager;
	Scene::IManager				*mSceneManager;
	TextureSpace::IManager		*mTextureManager;
};


}


#endif
