
// Header
#include "Manager.h"

// Library includes

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Core/IdGenerator.h>
#include <Interfaces/Material/IManager.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Shader/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Mesh/Format/3ds/Mesh3DSManager.h>
#include <Mesh/Format/md2/MeshMD2Manager.h>
#include <Mesh/Format/md5mesh/MeshMD5Manager.h>
#include <Mesh/Format/obj/MeshOBJManager.h>
#include <Mesh/Format/Particle/ParticleManager.h>
#include <Mesh/Format/ParticleSystem/ParticleSystemManager.h>
#include <Mesh/Format/PreparedObject/PreparedObjectManager.h>
#include <Mesh/Format/Sprite/SpriteManager.h>
#include <Mesh/Format/VolumeMesh/VolumeMeshManager.h>
#include <Renderer/RenderObject.h>
#include <Tools/Tools.h>

// Namespace declarations
using namespace Tools;


namespace Mesh {


ObjectManager::ObjectManager(const Common::Logger *p)
: Common::Logger(p, "ObjectManager"),
  mFormat("ao"),
  mIdGenerator(0),
  mMaterialManager(0),
  mMediaPathManager(0),
  mShaderManager(0),
  mTextureManager(0)
{
	debug("Starting...");
}

ObjectManager::~ObjectManager()
{
	shutdown();
}

void ObjectManager::addPlugin(IMeshManager *m)
{
	mManagers.push_back(m);
}

void ObjectManager::clear()
{
	debug("Clearing...");

	for ( MeshManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		if ( (*it) ) {
			(*it)->clear();
		}
	}

	mObjects.clear();
}

void ObjectManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mIdGenerator);
	assert(mMaterialManager);
	assert(mMediaPathManager);
	assert(mShaderManager);
	assert(mTextureManager);

	{	// Mesh3DSManager
		Mesh3DSManager *manager = new Mesh3DSManager(this);
		manager->configurationComplete();
		manager->init();

		addPlugin(manager);
	}
	{	// MeshMD2Manager
		MeshMD2Manager *manager = new MeshMD2Manager(this);
		manager->configurationComplete();
		manager->init();

		addPlugin(manager);
	}
	{	// MeshMD5Manager
		MeshMD5Manager *manager = new MeshMD5Manager(this);
		manager->configurationComplete();
		manager->init();

		addPlugin(manager);
	}
	{	// MeshOBJManager
		MeshOBJManager *manager = new MeshOBJManager(this);
		manager->configurationComplete();
		manager->init();

		addPlugin(manager);
	}
	{	// ParticleManager
		ParticleManager *manager = new ParticleManager(this);
		manager->connectMeshManager(this);
		manager->configurationComplete();
		manager->init();

		addPlugin(manager);
	}
	{	// ParticleSystemManager
		ParticleSystemManager *manager = new ParticleSystemManager(this);
		manager->connectMeshManager(this);
		manager->configurationComplete();
		manager->init();

		addPlugin(manager);
	}
	{	// PreparedObjectManager
		PreparedObjectManager *manager = new PreparedObjectManager(this);
		manager->connectMeshManager(this);
		manager->configurationComplete();
		manager->init();

		addPlugin(manager);
	}
	{	// SpriteManager
		SpriteManager *manager = new SpriteManager(this);
		manager->configurationComplete();
		manager->init();

		addPlugin(manager);
	}
	{	// VolumeMeshManager
		VolumeMeshManager *manager = new VolumeMeshManager(this);
		manager->configurationComplete();
		manager->init();

		addPlugin(manager);
	}

	mConfigurationComplete = true;
}

void ObjectManager::connectIdGenerator(Core::IdGenerator *g)
{
	mIdGenerator = g;
}

void ObjectManager::connectMaterialManager(MaterialSpace::IManager *m)
{
	mMaterialManager = m;
}

void ObjectManager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void ObjectManager::connectShaderManager(ShaderSpace::IManager *m)
{
	mShaderManager = m;
}

void ObjectManager::connectTextureManager(TextureSpace::IManager *m)
{
	mTextureManager = m;
}

Core::RenderObject* ObjectManager::create(const std::string& name)
{
	debug("create('" + name + "')");

	if ( name.empty() ) {
		warn("invalid name provided - aborting...");
		return 0;
	}

	std::string filename = Tools::toLowerCase( mMediaPathManager->buildObjectPath(Tools::Files::RemoveFileExt(name) + ".ao") );

	Core::Configuration::Configurator config(this, filename);
	if ( !config.success() ) {
		error("could not load configuration for '" + filename + "'!");
		return 0;
	}

	std::string mesh = config.getValue("mesh").toString();
	std::string type = config.getValue("type").toString();

	Core::RenderObject *object = find(filename);
	if ( object ) {
		debug("mesh '" + filename + "' already loaded, getting from repository");
		return object;
	}

	object = createFromPlugin(mesh, type);
	if ( !object ) {
		error("could not create RenderObject from '" + mesh + "'!");
		return 0;
	}

	{	// mesh setup
		object->name(filename);
		object->setId(mIdGenerator->generate());
		{
			object->animationSpeed(config.getValue("animspeed").toFloat());
			object->setColor(config.getValue("color").toVector3f());
			object->setColorMap(mTextureManager->create(config.getValue("colormap").toString(), mMediaPathManager->getMeshPath(mesh)));
			object->setFacing(static_cast<Core::Facing::E>(config.getValue("facing").toInt()));
			object->setHeightMap(mTextureManager->create(config.getValue("heightmap").toString(), mMediaPathManager->getMeshPath(mesh)));
			object->setMaterial(mMaterialManager->create(config.getValue("material").toString(), mMediaPathManager->getMeshPath(mesh)));
			object->setNormalMap(mTextureManager->create(config.getValue("normalmap").toString(), mMediaPathManager->getMeshPath(mesh)));
			object->setRotation(config.getValue("rotation").toVector3f());
			object->setScale(config.getValue("scale").toVector3f());
			object->setShader(mShaderManager->create(config.getValue("shader").toString(), mMediaPathManager->getMeshPath(mesh)));
			object->setSize(config.getValue("size").toVector3f());
			object->setTexturePack(mTextureManager->createPack(config.getValue("pack").toString(), mMediaPathManager->getMeshPath(mesh)));
			object->setTranslation(config.getValue("translation").toVector3f());
			object->setVolumeType(static_cast<IVolume::Type::E>(config.getValue("volumetype").toInt()));
		}
		object->init();				// Initialize RenderObject
	}

	mObjects.push_back(object);	// Add to our object list
	return object;
}

Core::RenderObject* ObjectManager::createFromPlugin(const std::string& filename, const std::string& extension)
{
	debug("createFromPlugin('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	std::string file_long;
	file_long += mMediaPathManager->getGame();
	file_long += mMediaPathManager->getMaps();
	file_long += mMediaPathManager->getMap();
	file_long += mMediaPathManager->getMeshes();
	file_long += filename + "/";
	file_long += "mesh." + extension;

	std::string file_short;
	file_short += mMediaPathManager->getGame();
	file_short += mMediaPathManager->getMeshes();
	file_short += filename + "/";
	file_short += "mesh." + extension;

	for ( MeshManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		if ( (*it)->handleFormat(extension) ) {
			std::string file;

			if ( Files::exists(file_long) ) {
				file = file_long;
			}
			else if ( Files::exists(file_short) ) {
				file = file_short;
			}
			else {
				error("mesh '" + filename + "' not found!");
				return 0;
			}

			return (*it)->create(file);
		}
	}

	return 0;
}

Core::RenderObject* ObjectManager::find(const std::string& filename)
{
	if ( filename.empty() ) {
		return 0;
	}

	for ( RenderObjectList::iterator it = mObjects.begin(); it != mObjects.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), filename) ) {
			return (*it);
		}
	}

	return 0;
}

bool ObjectManager::handleFormat(const std::string& format) const
{
	return Tools::StringCompareI(mFormat, format);
}

void ObjectManager::init()
{
	debug("Initializing...");

	mIsReady = true;
}

void ObjectManager::manage()
{
	// nothing to do here
}

void ObjectManager::shutdown()
{
	debug("Stopping...");

	clear();

	for ( MeshManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		if ( (*it) ) {
			delete (*it);
		}
	}

	mManagers.clear();
}

void ObjectManager::update(float elapsedTime)
{
	for ( MeshManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		(*it)->update(elapsedTime);
	}
}


}
