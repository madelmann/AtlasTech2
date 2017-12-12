
// Header
#include "Manager.h"

// Library includes

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Material/IManager.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Shader/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Parsers/LineParser.h>
#include <Scene/SceneGraphObject.h>
#include <Water/Plugins/ConsolePlugin.h>
#include "Water.h"

// Namespace declarations


namespace WaterSpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "WaterSpace::Manager"),
  mConsole(0),
  mMaterialManager(0),
  mMediaPathManager(0),
  mSceneManager(0),
  mShaderManager(0),
  mTextureManager(0)
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

	for ( WaterList::iterator it = mWater.begin(); it != mWater.end(); ++it ) {
		delete (*it);
	}
	mWater.clear();

	mSettings.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mConsole);
	assert(mMaterialManager);
	assert(mMediaPathManager);
	assert(mSceneManager);
	assert(mShaderManager);
	assert(mTextureManager);

	ConsolePlugin *plugin = new ConsolePlugin();
	plugin->connectSettings(&mSettings);

	mConsole->registerPlugin(plugin);

	mConfigurationComplete = true;
}

void Manager::connectConsole(IConsoleInterface *c)
{
	mConsole = c;
}

void Manager::connectMaterialManager(MaterialSpace::IManager *m)
{
	mMaterialManager = m;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void Manager::connectSceneManager(Scene::IManager *m)
{
	mSceneManager = m;
}

void Manager::connectShaderManager(ShaderSpace::IManager *m)
{
	mShaderManager = m;
}

void Manager::connectTextureManager(TextureSpace::IManager *m)
{
	mTextureManager = m;
}

void Manager::init()
{
	debug("Initializing...");

	std::string file = mMediaPathManager->buildConfigPath("water.conf");

	Core::Configuration::Configurator config(this, file);
	if ( !config.success() ) {
		return;
	}

	mSettings.color(config.getValue("color").toVector4f());
	mSettings.enabled(config.getValue("enabled").toBool());
	mSettings.facing(static_cast<Core::Facing::E>(config.getValue("facing").toInt()));
	mSettings.flowSpeed(config.getValue("flowspeed").toVector2f());
	mSettings.useNormals(config.getValue("normals").toBool());
	mSettings.position(config.getValue("position").toVector3f());
	mSettings.reflectionDepth(config.getValue("reflection_depth").toFloat());
	mSettings.useReflections(config.getValue("reflections").toBool());
	mSettings.useRefractions(config.getValue("refractions").toBool());
	mSettings.size(config.getValue("size").toVector3f());
	mSettings.waveHeight(config.getValue("waveheight").toFloat());
	mSettings.waveInterval(config.getValue("waveinterval").toFloat());

	Water *water = new Water();
	water->setColorMap(mTextureManager->create(config.getValue("colormap").toString()));
	water->setDuDvMap(mTextureManager->create(config.getValue("dudvmap").toString()));
	water->setHeightMap(mTextureManager->create(config.getValue("heightmap").toString()));
	water->setMaterial(mMaterialManager->create(config.getValue("material").toString()));
	water->setNormalMap(mTextureManager->create(config.getValue("normalmap").toString()));
	water->setShader(mShaderManager->create(config.getValue("shader").toString()));

	water->connectSettings(&mSettings);
	water->init();

/*	// our water is rendered directly from our water manager
	Scene::SceneGraphObject *sgo = mSceneManager->add(water);
	sgo->setPosition(mSettings.position());
	mSceneManager->attach(sgo, Core::invalidID, true);
*/
	mWater.push_back(water);

	mIsReady = true;
}

void Manager::manage()
{
	// nothing to do here
}

Settings* Manager::provideSettings()
{
	return &mSettings;
}

void Manager::render()
{
	for ( WaterList::const_iterator it = mWater.begin(); it != mWater.end(); it++ ) {
		(*it)->render();
	}
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::update(float elapsedTime)
{
	for ( WaterList::iterator it = mWater.begin(); it != mWater.end(); it++ ) {
		(*it)->update(elapsedTime);
	}
}


}
