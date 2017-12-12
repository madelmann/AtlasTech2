
#ifndef _Light_Manager_h_
#define _Light_Manager_h_


// Library includes
#include <list>
#include <string>

// Project includes
#include "Settings.h"
#include <Common/Logger.h>
#include <Interfaces/Light/IManager.h>

// Forward declarations
namespace Console {
	class IConsoleInterface;
}
class IMediaPathManager;

// Namespace declarations
using Console::IConsoleInterface;


namespace LightSpace {

// Forward declarations
class Light;

class Manager : public IManager,
					 private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

	// IManager implementation
	//{
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime = 0.f);
	//}

	// Connectors
	//{
	void connectConsole(IConsoleInterface *c);
	void connectMediaPathManager(IMediaPathManager *m);
	//}

	// Providers
	//{
	Settings* provideSettings();
	//}

	Light* create(const std::string& name);

protected:

private:
	typedef std::list<Light*> LightList;

private:
	Settings	mSettings;

	IConsoleInterface	*mConsole;
	IMediaPathManager	*mMediaPathManager;

	LightList	mLights;
};


}


#endif
