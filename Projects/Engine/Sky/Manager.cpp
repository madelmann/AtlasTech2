
// Header
#include "Manager.h"

// Library includes

// Project includes
#include "ConsolePlugin.h"
#include "SkyBox.h"
#include <Core/Configuration/Configurator.h>
#include <Core/IdGenerator.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Scene/SceneGraphObject.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace SkySpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "SkySpace::Manager"),
  mConsole(0),
  mIdGenerator(0),
  mMediaPathManager(0),
  mSceneManager(0),
  mSky(0),
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
	debug("Clearing..");

	aeDelete( mSky );

	mIsReady = false;
}

void Manager::configurationComplete()
{
	debug("configurationComplete");

	assert(!mConfigurationComplete);

	assert(mConsole);
	assert(mIdGenerator);
	assert(mMediaPathManager);
	assert(mSceneManager);
	assert(mTextureManager);

	ConsolePlugin *plugin = new ConsolePlugin();
	plugin->connectSettings(&mSettings);

	mConsole->registerPlugin(plugin);

	mConfigurationComplete = true;
}

void Manager::connectConsole(Console::IConsoleInterface *c)
{
	mConsole = c;
}

void Manager::connectIdGenerator(Core::IdGenerator *g)
{
	mIdGenerator = g;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
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

	std::string file = mMediaPathManager->buildConfigPath("sky.conf");

	Core::Configuration::Configurator configurator(this, file);
	if ( !configurator.success() ) {
		return;
	}

	mSettings.affectedByFog(configurator.getValue("affected_by_fog").toBool());
	mSettings.affectedByLight(configurator.getValue("affected_by_light").toBool());
	mSettings.fileFormat(configurator.getValue("fileformat").toString());
	mSettings.position(configurator.getValue("position").toVector3f());
	mSettings.size(configurator.getValue("size").toVector3f());
	mSettings.theme(configurator.getValue("theme").toString());
	mSettings.useFaceCulling(configurator.getValue("use_face_culling").toBool());

	mSky = new SkyBox(mIdGenerator->generate());
	mSky->connectSettings(&mSettings);

	std::string skyfolder = "sky";
	if ( !Tools::Files::exists(mMediaPathManager->buildTexturePath(skyfolder + "/" + mSettings.theme() + "/back." + mSettings.fileFormat())) ) {
		skyfolder = "skybox";
	}

	SkyBox *sky = static_cast<SkyBox*>(mSky);
	{
		sky->setBack(
			mTextureManager->create(skyfolder + "/" + mSettings.theme() + "/back." + mSettings.fileFormat())
		);
		sky->setBottom(
			mTextureManager->create(skyfolder + "/" + mSettings.theme() + "/bottom." + mSettings.fileFormat())
		);
		sky->setFront(
			mTextureManager->create(skyfolder + "/" + mSettings.theme() + "/front." + mSettings.fileFormat())
		);
		sky->setLeft(
			mTextureManager->create(skyfolder + "/" + mSettings.theme() + "/left." + mSettings.fileFormat())
		);
		sky->setRight(
			mTextureManager->create(skyfolder + "/" + mSettings.theme() + "/right." + mSettings.fileFormat())
		);
		sky->setTop(
			mTextureManager->create(skyfolder + "/" + mSettings.theme() + "/top." + mSettings.fileFormat())
		);
	}

	mSky->init();

	//Scene::SceneGraphObject *sgo = mSceneManager->add(mSky);
	//sgo->init(mSettings.position(), vector3f());
	//mSceneManager->attach(sgo, Core::invalidID, true);

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
	if ( !mIsReady ) {
		// we are not initialized!
		return;
	}

	mSky->render();
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::update(float elapsedTime)
{
	mSky->update(elapsedTime);
}


}
