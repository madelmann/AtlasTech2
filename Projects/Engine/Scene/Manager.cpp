
// Header
#include "Manager.h"

// Library includes

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Core/EngineSettings.h>
#include <Core/IdGenerator.h>
#include <Interfaces/Camera/IViewer.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Core/ICoreUtilities.h>
#include <Interfaces/Driver/IDriver.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Terrain/IManager.h>
#include <Math/Vector3.h>
#include <Parsers/ConfigParser.h>
#include <Parsers/LineParser.h>
#include <Renderer/RenderObject.h>
#include <Scene/Plugins/ConsolePlugin.h>
#include <Scene/Plugins/ViewFrustumRenderer.h>
#include <Tools/Tools.h>
#include "Scene.h"
#include "SceneGraphObject.h"
#include "SceneObjectParser.h"

// Namespace declarations
using namespace Tools;


namespace Scene {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "Scene::Manager"),
  mConsole(0),
  mCoreUtilities(0),
  mDriver(0),
  mEngineSettings(0),
  mFrustum(0),
  mGenerator(0),
  mLastOcclusionQuery(0.f),
  mMediaPathManager(0),
  mObjectManager(0),
  mScene(0),
  mTerrainManager(0),
  mViewer(0)
{
	debug("Starting...");
}

Manager::~Manager()
{
	shutdown();
}

bool Manager::add(const std::string& entity)
{
	if ( entity.empty() ) {
		return false;
	}

	std::string mesh;
	std::string position;
	std::string rotation;

	unsigned int c = 0;

	for ( ++c; c < entity.length() - 1; c++ ) {
		if ( entity[c] == '|' ) {
			break;
		}

		mesh += entity[c];
	}
	for ( ++c; c < entity.length() - 1; c++ ) {
		if ( entity[c] == '|' ) {
			break;
		}
		
		position += entity[c];
	}
	for ( ++c; c < entity.length() - 1; c++ ) {
		rotation += entity[c];
	}

	Value value = LineParser::parse(mesh);
	Core::RenderObject *obj = mObjectManager->create(value.toString());

	if ( !obj ) {
		warn("could not load mesh '" + mesh + "'!");
		return false;
	}

	SceneGraphObject *sgo = create(obj);
	if ( !sgo ) {
		warn("could add mesh '" + mesh + "' to scene!");
		return false;
	}

	value = LineParser::parse(position);
	sgo->setPosition(value.toVector3f());

	value = LineParser::parse(rotation);
	sgo->setRotation(value.toVector3f());

	attach(sgo, Core::invalidID, true);

	return true;
}

void Manager::attach(SceneGraphObject *child, Core::AEid parent, bool keepPosition)
{
	if ( !child ) {
		return;
	}

	SceneGraphObject *finder = 0;
	if ( parent != Core::invalidID ) {
		finder = getById(parent);
		if ( !finder ) {
			warn("no <SceneGraphObject> found with Id " + Tools::toString(parent) + " - aborting...");
			return;
		}
	}

	attach(child, finder, keepPosition);
}

void Manager::attach(SceneGraphObject *child, SceneGraphObject *parent, bool keepPosition)
{
	if ( !child ) {
		return;
	}

	if ( parent ) {
		parent->attach(child, keepPosition);
	}
	else {
		mScene->attach(child, true);
	}

	mSceneGraphObjects.push_back(child);
}

void Manager::clear()
{
	debug("Clearing...");

	mSettings.clear();

	aeDelete(mScene);

	for ( SceneGraphObjects::iterator it = mSceneGraphObjects.begin(); it != mSceneGraphObjects.end(); ++it ) {
		aeDelete((*it));
	}
	mSceneGraphObjects.clear();
}

SceneGraphObject* Manager::collidePoint(const vector3f& /*vector*/, bool /*child*/)
{
	if ( mSceneGraphObjects.empty() ) {
		return 0;
	}

	for ( SceneGraphObjects::iterator it = mSceneGraphObjects.begin(); it != mSceneGraphObjects.end(); ++it ) {
		//Core::RenderObject *object = (*it).collidePoint(vector, child);

		//if ( object ) {
		//	return object;
		//}
	}

	return 0;
}

SceneGraphObject* Manager::create(Core::RenderObject *ro)
{
	assert(mGenerator);

	if ( !mScene ) {
		fatal("scene not yet initialized!");
		return 0;	// in case 'fatal' does not quit our application
	}
	if ( !ro ) {
		warn("invalid <RenderObject> provided!");
		return 0;
	}

	SceneGraphObject *sgo = new SceneGraphObject(mGenerator->generate());
	sgo->setRenderObject(ro);

	return sgo;
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mConsole);
	assert(mCoreUtilities);
	assert(mDriver);
	assert(mEngineSettings);
	assert(mFrustum);
	assert(mGenerator);
	assert(mMediaPathManager);
	assert(mObjectManager);
	assert(mTerrainManager);

	mSceneObjectParser = new SceneObjectParser(this);

	{	// Console plugin
		ConsolePlugin *plugin = new ConsolePlugin();
		plugin->connectObjectManager(mObjectManager);
		plugin->connectSceneManager(this);
		plugin->connectTerrainManager(mTerrainManager);

		mConsole->registerPlugin(plugin);
	}
	{	// Render plugin
		ViewFrustumRenderer *renderPlugin = new ViewFrustumRenderer();
		renderPlugin->connectFrustum(mFrustum);

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

void Manager::connectDriver(IDriver *d)
{
	mDriver = d;
}

void Manager::connectEngineSettings(Core::EngineSettings *s)
{
	mEngineSettings = s;
}

void Manager::connectFrustum(Frustum *f)
{
	mFrustum = f;
}

void Manager::connectIdGenerator(Core::IdGenerator *g)
{
	mGenerator = g;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void Manager::connectObjectManager(Mesh::IObjectManager *m)
{
	mObjectManager = m;
}

void Manager::connectTerrainManager(Terrain::IManager *m)
{
	mTerrainManager = m;
}

void Manager::detach(Core::AEid id)
{
	SceneGraphObject *sgo = getById(id);

	detach(sgo);
}

void Manager::detach(SceneGraphObject *sgo)
{
	if ( !sgo ) {
		return;
	}

	// Detach from scene ...
	mScene->detach(sgo);

	// ... and remove from "repo"
	mSceneGraphObjects.remove(sgo);
}

void Manager::erase(SceneGraphObject *sgo)
{
	if ( !sgo ) {
		return;
	}

	// Detach from scene ...
	detach(sgo);

	// ... and delete
	aeDelete(sgo);
}

SceneGraphObject* Manager::getById(Core::AEid id)
{
	if ( id == Core::invalidID ) {
		return 0;
	}

	for ( SceneGraphObjects::iterator it = mSceneGraphObjects.begin(); it != mSceneGraphObjects.end(); ++it ) {
		if ( (*it)->id() == id ) {
			return (*it);
		}
	}

	return 0;
}

SceneGraphObject* Manager::getByName(const std::string& name)
{
	if ( name.empty() ) {
		return 0;
	}

	for ( SceneGraphObjects::iterator it = mSceneGraphObjects.begin(); it != mSceneGraphObjects.end(); ++it ) {
		if ( (*it)->name() == name ) {
			return (*it);
		}
	}

	return 0;
}

Scene* Manager::getScene()
{
	return mScene;
}

void Manager::init()
{
	debug("Initializing...");

	mSettings.distanceFade(mEngineSettings->getMaxFadeViewDepth());
	mSettings.distanceHide(mEngineSettings->getMaxViewDepth());
	mSettings.setEditorMode(mEngineSettings->isEditorMode());

	mScene = new Scene(this);
	mScene->connectFrustum(mFrustum);
	mScene->connectSettings(&mSettings);
	mScene->connectViewer(mViewer);
	mScene->setId(mGenerator->generate());

	mIsReady = true;
}

bool Manager::load(const std::string& filename)
{
	debug("Loading scene '" + filename + "'...");

	if ( filename.empty() ) {
		error("invalid scene name provided!");
		return false;
	}

	std::string file = mMediaPathManager->buildConfigPath("scene.conf");

	Core::Configuration::Configurator config(this, file);
	if ( !config.success() ) {
		return false;
	}

	mScene->setEditorMode(mSettings._getEditorMode());
	mSettings.setDayTime(config.getValue("daytime").toInt());
	mViewer->position(config.getValue("camera.position").toVector3f());
	mViewer->view(config.getValue("camera.view").toVector3f());

	int size = config.getValue("entity.size").toInt();
	for ( int i = 0; i < size; ++i ) {
		add(config.getValue("entity", i).toString());
	}

	return true;
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

	assert(mScene);
	mScene->render();
}

SceneGraphObject* Manager::replicate(SceneGraphObject *sgo)
{
	if ( !sgo ) {
		return 0;
	}

	SceneGraphObject *copy = new SceneGraphObject(mGenerator->generate());
	copy->alwaysVisible(sgo->alwaysVisible());
	copy->collisionIdentifier(sgo->collisionIdentifier());
	copy->setParent(sgo->getParent());
	copy->setRenderObject(sgo->getRenderObject());
	copy->setScale(sgo->getScale());
	copy->setSize(sgo->getSize());

	copy->init(sgo->getPosition(), sgo->getRotation());

	if ( copy->getParent() ) {
		attach(copy, copy->getParent()->id(), true);
	}

	return copy;
}

void Manager::setViewer(CameraSpace::IViewer *v)
{
	mViewer = v;
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();

	delete mSceneObjectParser;
}

void Manager::update(float elapsedTime)
{
	if ( !mScene ) {
		return;
	}

	mScene->update(elapsedTime);

	if ( mEngineSettings->useOcclusionQuery() ) {
		mLastOcclusionQuery += elapsedTime;

		if ( mLastOcclusionQuery > mEngineSettings->getTimeBetweenOcclusionQueries() ) {
			updateOcclusionQuerys();
		}
	}
}

void Manager::updateOcclusionQuerys()
{
	mLastOcclusionQuery = 0.f;

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	aeDepthMask(GL_FALSE);

	mScene->updateOcclusionQuery(mDriver);

	glFlush();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	aeDepthMask(GL_TRUE);
}


}
