
#ifndef Fog_Manager_h
#define Fog_Manager_h


// Library includes

// Project includes
#include "Fog.h"
#include "Settings.h"
#include <Common/Logger.h>
#include <Interfaces/Fog/IManager.h>

// Forward declarations
namespace Console {
	class IConsoleInterface;
}
class IMediaPathManager;

// Namespace declarations
using Console::IConsoleInterface;


namespace FogSpace {

// Forward declarations

class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

public:
	// IManager implementation
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
	void connectConsole(IConsoleInterface *c);
	void connectMediaPathManager(IMediaPathManager *m);
	//}

	// Providers
	//{
	Fog* provideFog();
	Settings* provideSettings();
	//}

	void set();
	void unset();

protected:

private:
	Fog			mFog;
	Settings	mSettings;

	IConsoleInterface	*mConsole;
	IMediaPathManager	*mMediaPathManager;
};


}


#endif
