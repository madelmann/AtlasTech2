
// Header
#include "Manager.h"

// Library includes

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Light/Plugins/ConsolePlugin.h>
#include <Parsers/LineParser.h>
#include <Tools/Files.h>
#include "Light.h"

// Namespace declarations
using namespace Tools;


namespace LightSpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "LightSpace::Manager"),
  mConsole(0),
  mMediaPathManager(0)
{
	debug("Starting...");
}

Manager::~Manager()
{
	shutdown();
}

void Manager::clear()
{
	debug("Clearing...");

	mSettings.clear();

	for ( LightList::iterator it = mLights.begin(); it != mLights.end(); ++it ) {
		delete (*it);
	}

	mLights.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mConsole);
	assert(mMediaPathManager);

	ConsolePlugin *plugin = new ConsolePlugin();
	plugin->connectSettings(&mSettings);

	mConsole->registerPlugin(plugin);

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

Light* Manager::create(const std::string& name)
{
	debug("create('" + name + "')");

	if ( name.empty() ) {
		//warn("create(): invalid name provided!");
		return 0;
	}

	Light *light = new Light(this);

	// do some things...

	mLights.push_back(light);

	return light;
}

void Manager::init()
{
	debug("Initializing...");

	std::string file = mMediaPathManager->buildConfigPath("light.conf");

	Core::Configuration::Configurator config(this, file);
	if ( !config.success() ) {
		return;
	}

	mSettings.ambient(config.getValue("ambient").toVector4f());
	mSettings.attenuation(config.getValue("attenuation").toVector3f());
	mSettings.constantAttenuation(config.getValue("attenuation_constant").toFloat());
	mSettings.diffuse(config.getValue("diffuse").toVector4f());
	mSettings.direction(config.getValue("direction").toVector3f());
	mSettings.enabled(config.getValue("enabled").toBool());
	mSettings.linearAttenuation(config.getValue("attenuation_linear").toFloat());
	mSettings.mode(static_cast<LightType::E>(config.getValue("mode").toInt()));
	mSettings.position(config.getValue("position").toVector3f());
	mSettings.quadraticAttenuation(config.getValue("attenuation_quadratic").toFloat());
	mSettings.specular(config.getValue("specular").toVector4f());

	mIsReady = true;
}

void Manager::manage()
{
	// nothing todo here
}

Settings* Manager::provideSettings()
{
	return &mSettings;
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::update(float /*elapsedTime*/)
{
	// nothing todo here
}


}
