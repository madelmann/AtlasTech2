
// Header
#include "Manager.h"

// Library includes 

// Project includes
#include <Core/EngineSettings.h>
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Core/ICoreUtilities.h>
#include <Interfaces/Material/IManager.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Physics/IManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Shader/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Parsers/ConfigParser.h>
#include <Parsers/LineParser.h>
#include <Scene/SceneGraphObject.h>
#include <Terrain/Plugins/ConsolePlugin.h>
#include <Terrain/Plugins/RenderPlugin.h>
#include <Terrain/Subdivided/Layer.h>
#include <Terrain/VBO/Layer.h>

// Namespace declarations


namespace Terrain {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "Terrain::Manager"),
  mConsole(0),
  mCoreUtilities(0),
  mEngineSettings(0),
  mMaterialManager(0),
  mMediaPathManager(0),
  mPhysicManager(0),
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

bool Manager::add(const std::string & filename)
{
	if ( filename.empty() ) {
		// return silently
		return false;
	}

	Core::Configuration::Configurator config(this, mMediaPathManager->buildTerrainPath(filename));
	if ( !config.success() ) {
		return false;
	}

	ALayer *layer = 0;
	if ( mSettings.useSubdividedLayers() ) {
		layer = new Subdivided::Layer(this);
	}
	else {
		layer = new VBO::Layer(this);
		static_cast<VBO::Layer*>(layer)->connectPhysicManager(mPhysicManager);
	}
	layer->connectEngineSettings(mEngineSettings);
	layer->connectMediaPathManager(mMediaPathManager);
	layer->connectSceneManager(mSceneManager);
	layer->connectSettings(&mSettings);

	layer->setAdditionalTexture( mTextureManager->create(config.getValue("additionaltexture").toString()) );
	layer->setAlphaTexture( mTextureManager->create(config.getValue("alphatexture").toString()) );
	layer->setBaseTexture( mTextureManager->create(config.getValue("basetexture").toString()) );
	layer->setColor(config.getValue("color").toVector3f());
	layer->enabled(config.getValue("enabled").toBool());
	layer->setFacing( Core::Facing::convert(config.getValue("facing").toInt()) );
	layer->setHeightMap(config.getValue("heightmap").toString());
	layer->setMaterial( mMaterialManager->create(config.getValue("material").toString()) );
	layer->setNormalMap( mTextureManager->create(config.getValue("normalmap").toString()) );
	layer->setOffset(config.getValue("offset").toFloat());
	layer->setScale(mSettings.scale());
	layer->setShader( mShaderManager->create(config.getValue("shader").toString()) );
	layer->setSubdivisions(mSettings.subdivisions());

	int texId = 0;
	while ( config.isMember("texture", texId, "enabled") ) {
		if ( !config.getValue("texture", texId, "enabled").toBool() ) {
			texId++;
			continue;
		}

		bool enabled = config.getValue("texture", texId, "enabled").toBool();
		std::string colormap = config.getValue("texture", texId, "colormap").toString();
		vector2f scale = config.getValue("texture", texId, "scale").toVector2f();

		ALayer::TextureSettings s;
		s.Enabled = enabled;
		s.ColorMap = mTextureManager->create(colormap);
		s.Scale = scale;

		layer->addTexture(s);

		texId++;
	}


	if ( !layer->build() ) {
		warn("Layer '" + filename + "' build failed!");
		delete layer;
		return false;
	}

	debug("add('" + filename + "')");

/*	// the terrain is rendered directly by our terrain manager
	Scene::SceneGraphObject *layer_sgo = mSceneManager->add(layer);
	//layer_sgo->attachPhysicalBody(layer->physicalObject());
	mSceneManager->attach(layer_sgo, Core::invalidID, true);
*/

	mLayers.push_back(layer);
	return true;
}

void Manager::clear()
{
	debug("Clearing...");

	for ( LayerList::iterator it = mLayers.begin(); it != mLayers.end(); it++ ) {
		delete (*it);
	}

	mLayers.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mConsole);
	assert(mCoreUtilities);
	assert(mEngineSettings);
	assert(mMaterialManager);
	assert(mMediaPathManager);
	assert(mPhysicManager);
	assert(mSceneManager);
	assert(mShaderManager);
	assert(mTextureManager);

	{	// Console plugin
		ConsolePlugin *consolePlugin = new ConsolePlugin();
		consolePlugin->connectSettings(&mSettings);

		mConsole->registerPlugin(consolePlugin);
	}
	{	// Render plugin
		RenderPlugin *renderPlugin = new RenderPlugin();
		renderPlugin->connectSettings(&mSettings);

		mCoreUtilities->registerPlugin(renderPlugin);
	}

	mConfigurationComplete = true;
}

void Manager::connectConsole(IConsoleInterface *c)
{
	mConsole = c;
}

void Manager::connectCoreUtilities(Core::ICoreUtilities *u)
{
	mCoreUtilities = u;
}

void Manager::connectEngineSettings(Core::EngineSettings *s)
{
	mEngineSettings = s;
}

void Manager::connectMaterialManager(MaterialSpace::IManager *m)
{
	mMaterialManager = m;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void Manager::connectPhysicManager(Physics::IManager *m)
{
	mPhysicManager = m;
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

vector3f Manager::heightAt(const vector3f& pos)
{
	vector3f result;

	for ( LayerList::const_iterator it = mLayers.begin(); it != mLayers.end(); it++ ) {
		result = (*it)->heightAt(pos);

		if ( result.x != 0.f || result.y != 0.f || result.z != 0 ) {
			return result;
		}
	}

	return pos;
}

void Manager::init()
{
	debug("Initializing...");

	Core::Configuration::Configurator config(this, mMediaPathManager->buildConfigPath("terrain.conf"));
	if ( !config.success() ) {
		return;
	}

	mSettings.debugMode(config.getValue("debug_enabled").toBool());
	mSettings.enabled(config.getValue("enabled").toBool());
	mSettings.renderNormals(config.getValue("debug_render_normals").toBool());
	mSettings.scale(config.getValue("scale").toVector3f());
	mSettings.subdivisions(config.getValue("subdivisions").toVector2f());

	if ( mSettings.enabled() ) {
		load();
	}

	mIsReady = true;
}

bool Manager::load()
{
	debug("Loading...");

	int layer = 0;
	while ( add("layer" + Tools::toString(layer) + ".conf") ) { 
		layer++;
	}

	return true;
}

void Manager::manage()
{
	// nothing to do here
}

vector3f Manager::normalAt(const vector3f& pos)
{
	vector3f result;

	for ( LayerList::const_iterator it = mLayers.begin(); it != mLayers.end(); it++ ) {
		result = (*it)->normalAt(pos);

		if ( result.x != 0.f || result.y != 0.f || result.z != 0 ) {
			return result;
		}
	}

	return vector3f();
}

Settings* Manager::provideSettings()
{
	return &mSettings;
}

void Manager::render()
{
	for ( LayerList::const_iterator it = mLayers.begin(); it != mLayers.end(); it++ ) {
		Core::RenderObject *ro = (*it);
		ro->render();
	}
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::update(float elapsedTime)
{
	for ( LayerList::iterator it = mLayers.begin(); it != mLayers.end(); it++ ) {
		Core::RenderObject *ro = (*it);
		ro->update(elapsedTime);
	}
}


}
