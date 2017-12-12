
// Header
#include "Manager.h"

// Library includes

// Project includes
#include "ConsolePlugin.h"
#include <Core/EngineStates.h>
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Material/IManager.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Renderer/RenderObject.h>
#include <Scene/SceneGraphObject.h>
#include <Tools/Files.h>

// Namespace declarations
using namespace Tools;


namespace CloudSpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "CloudSpace::Manager"),
  mConsole(0),
  mEngineStates(0),
  mMaterialManager(0),
  mMediaPathManager(0),
  mMeshManager(0),
  mSceneManager(0),
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

	mSettings.clear();

	mClouds.clear();
	mSceneClouds.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mConsole);
	assert(mEngineStates);
	assert(mMaterialManager);
	assert(mMediaPathManager);
	assert(mMeshManager);
	assert(mSceneManager);
	assert(mTextureManager);

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

void Manager::connectEngineStates(EngineStates *s)
{
	mEngineStates = s;
}

void Manager::connectMaterialManager(MaterialSpace::IManager *m)
{
	mMaterialManager = m;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void Manager::connectMeshManager(Mesh::IObjectManager *m)
{
	mMeshManager = m;
}

void Manager::connectSceneManager(Scene::IManager *m)
{
	mSceneManager = m;
}

void Manager::connectTextureManager(TextureSpace::IManager *m)
{
	mTextureManager = m;
}

void Manager::init()
{
	debug("Initializing...");

	std::string file = mMediaPathManager->buildConfigPath("clouds.conf");

	Core::Configuration::Configurator config(this, file);
	if ( !config.success() ) {
		return;
	}

	mSettings.color(config.getValue("color").toVector3f());
	mSettings.colorMap(config.getValue("colormap").toString());
	mSettings.count(config.getValue("count").toInt());
	mSettings.enabled(config.getValue("enabled").toBool());
	mSettings.end(config.getValue("end").toVector3f());
	mSettings.height(config.getValue("height").toFloat());
	mSettings.material(config.getValue("material").toString());
	mSettings.scale(config.getValue("scale").toVector3f());
	mSettings.sizeMax(config.getValue("size.max").toVector3f());
	mSettings.sizeMin(config.getValue("size.min").toVector3f());
	mSettings.start(config.getValue("start").toVector3f());

	if ( mSettings.enabled() ) {
		Core::RenderObject *cloud = mMeshManager->create("cloud.ao");

		assert(cloud);

		cloud->setColor(mSettings.color());
		cloud->setColorMap(mTextureManager->create(mSettings.colorMap()));
		cloud->setMaterial(mMaterialManager->create(mSettings.material()));
		cloud->setScale(mSettings.scale());
		cloud->setSize(mSettings.sizeMax());

		cloud->init();

		mClouds.push_back(cloud);

		for ( int i = 0; i < mSettings.count(); i += 1 ) {
			Scene::SceneGraphObject *sgo = mSceneManager->create(cloud);
			if ( !sgo ) {
				fatal("could not add cloud to scene!");
				return;
			}

			sgo->init(vector3f(i * 300.f, mSettings.height(), i * 300.f), vector3f());

			mSceneManager->attach(sgo, Core::invalidID, true);

			mSceneClouds.push_back(sgo);
		}
	}

	mIsReady = true;
}

void Manager::manage()
{
	for ( SceneCloudList::iterator it = mSceneClouds.begin(); it != mSceneClouds.end(); ++it ) {
		// Move cloud with wind
		vector3f newPos = (*it)->getPosition();
		//vector3f wind = mEngineStates->getWind();
		//vector3f posAdd;
		//posAdd.x = wind.x * mEngineStates->getFrameTime();
		//posAdd.y = wind.y * mEngineStates->getFrameTime();
		//posAdd.z = wind.z * mEngineStates->getFrameTime();
		//newPos += wind;

		if ( newPos.x > mSettings.end().x ) {
			newPos = vector3f(0.f, newPos.y, newPos.z);
		}
		if ( newPos.z > mSettings.end().z ) {
			newPos = vector3f(newPos.x, newPos.y, 0.f);
		}

		(*it)->setPosition(newPos);
	}
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

void Manager::update(float elapsedTime)
{
	static float speed = 10.f;

	for ( SceneCloudList::iterator it = mSceneClouds.begin(); it != mSceneClouds.end(); it++ ) {
		(*it)->update(elapsedTime);

		vector3f pos = (*it)->getPosition();
		pos.x += speed * elapsedTime;

		(*it)->setPosition(pos);
	}
}


}
