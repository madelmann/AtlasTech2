
// Header
#include "Wind.h"

// Library includes

// Project includes
#include "ConsolePlugin.h"
#include <Interfaces/Console/IConsoleInterface.h>

// Namespace declarations


namespace WindSpace {


Wind::Wind(const Common::Logger *p)
: Common::Logger(p, "Wind"),
  mConsole(0)
{
	debug("Starting...");
}

Wind::~Wind()
{
	debug("Stopping...");

	clear();
}

void Wind::clear()
{
	debug("Clearing...");

	mSettings.clear();
}

void Wind::connectConsole(IConsoleInterface *c)
{
	assert(c);
	assert(!mConsole);

	mConsole = c;
}

void Wind::init()
{
	debug("Initializing...");

	assert(mConsole);

	ConsolePlugin *plugin = new ConsolePlugin();
	plugin->connectWind(this);

	mConsole->registerPlugin(plugin);

	clear();
}

void Wind::manage()
{
}

Settings& Wind::provideSettings()
{
	return mSettings;
}

void Wind::reset()
{
	debug("Reseting...");

	clear();
	//init();
}

void Wind::update(float /*elapsedTime*/)
{
}


}
