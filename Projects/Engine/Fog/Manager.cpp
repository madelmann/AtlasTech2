
// Header
#include "Manager.h"

// Library includes

// Project includes
#include "ConsolePlugin.h"
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Tools/Tools.h>

// Namespace declarations
using namespace Tools;


namespace FogSpace {


Manager::Manager(const Common::Logger *p)
: Logger(p, "FogSpace::Manager"),
  mConsole(0),
  mFog(&mSettings),
  mMediaPathManager(0)
{
}

Manager::~Manager()
{
	shutdown();
}

void Manager::clear()
{
	mSettings.clear();

	set();
}

void Manager::configurationComplete()
{
	assert(!mConfigurationComplete);

	assert(mConsole);
	assert(mMediaPathManager);

	{	// Console plugin
		ConsolePlugin *plugin = new ConsolePlugin();
		plugin->connectSettings(&mSettings);

		mConsole->registerPlugin(plugin);
	}

	mConfigurationComplete = true;
}

void Manager::connectConsole(IConsoleInterface *c)
{
	mConsole = c;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void Manager::init()
{
	debug("Initializing...");

	std::string file = mMediaPathManager->buildConfigPath("fog.conf");

	Core::Configuration::Configurator configurator(this, file);
	if ( !configurator.success() ) {
		warn("could not load fog configuration from '" + file + "'!");
		return;
	}

	mSettings.affectsSkyBox(configurator.getValue("affectsskybox").toBool());
	mSettings.color(configurator.getValue("color").toVector4f());
	mSettings.density(configurator.getValue("density").toFloat());
	mSettings.enabled(configurator.getValue("enabled").toBool());
	mSettings.end(configurator.getValue("end").toFloat());
	mSettings.mode(FogMode::convert(configurator.getValue("mode").toInt()));
	mSettings.start(configurator.getValue("start").toFloat());

	mIsReady = true;
}

void Manager::manage()
{
	// nothing to do here
}

Fog* Manager::provideFog()
{
	return &mFog;
}

Settings* Manager::provideSettings()
{
	return &mSettings;
}

void Manager::set()
{
	mFog.bind();
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::unset()
{
	mFog.unbind();
}

void Manager::update(float /*elapsedTime*/)
{
	// nothing to do here
}


}
