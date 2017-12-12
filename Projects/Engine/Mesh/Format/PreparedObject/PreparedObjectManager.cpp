
// Header
#include "PreparedObjectManager.h"

// Library includes

// Project includes
#include "PreparedObject.h"
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


PreparedObjectManager::PreparedObjectManager(const Common::Logger *p)
: AMeshManager(p, "PreparedObjectManager"),
  mMeshManager(0)
{
	debug("Starting...");

	mFormat = "ppo";
}

PreparedObjectManager::~PreparedObjectManager()
{
	shutdown();
}

void PreparedObjectManager::clear()
{
	debug("Clearing...");

	for ( PreparedObjectList::iterator it = mPreparedObjects.begin(); it != mPreparedObjects.end(); it++ ) {
		delete (*it);
	}

	mPreparedObjects.clear();
}

void PreparedObjectManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mMeshManager);

	mConfigurationComplete = true;
}

void PreparedObjectManager::connectMeshManager(IObjectManager *m)
{
	mMeshManager = m;
}

Core::RenderObject* PreparedObjectManager::create(const std::string& filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}
	if ( !Tools::Files::exists(filename) ) {
		warn("create(): file '" + filename + "' does not exist!");
		return 0;
	}

	std::string meshName = Tools::Files::ExtractFilename(filename);

	Core::RenderObject *ro = find(meshName);
	if ( ro ) {
		debug("create(): mesh with name '" + meshName + "' already loaded.");
		return ro;
	}

	Core::Configuration::Configurator config(this, Tools::Files::ChangeFileExt(filename, "conf"));
	if ( !config.success() ) {
		error("could not load configuration for '" + filename + "'!");
		return 0;
	}

	PreparedObject *ppo = new PreparedObject();

	{	// mesh setup
		int size = config.getValue("leaves.size").toInt();
		for ( int c = 0; c < size; ++c ) {
			bool enabled = config.getValue("leaves", c, "enabled").toBool();
			std::string mesh = config.getValue("leaves", c, "mesh").toString();
			vector3f position = config.getValue("leaves", c, "position").toVector3f();
			//vector3f scale = config.getValue("leaves", c, "scale").toVector3f();

			Core::RenderObject *ro = mMeshManager->create(mesh);
			if ( !ro ) {
				warn("invalid <Core::RenderObject>: '" + mesh + "'");
				continue;
			}

			PreparedObject::Leaf l(mesh);
			l.enabled(enabled);
			l.mesh(ro);
			l.position(position);
			//l.scale(scale);

			ppo->addLeaf(l);
		}
	}

	mPreparedObjects.push_back(ppo);
	return ppo;
}

Core::RenderObject* PreparedObjectManager::find(const std::string& name)
{
	if ( name.empty() ) {
		return 0;
	}

	for ( PreparedObjectList::iterator it = mPreparedObjects.begin(); it != mPreparedObjects.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), name) ) {
			return (*it);
		}
	}

	return 0;
}

void PreparedObjectManager::update(float elapsedTime)
{
	for ( PreparedObjectList::iterator it = mPreparedObjects.begin(); it != mPreparedObjects.end(); it++ ) {
		(*it)->update(elapsedTime);
	}
}



}
