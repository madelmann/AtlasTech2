
// Header
#include "ParticleSystemManager.h"

// Library includes

// Project includes
#include "ParticleSystem.h"
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


ParticleSystemManager::ParticleSystemManager(const Common::Logger *p)
: AMeshManager(p, "ParticleSystemManager"),
  mMeshManager(0)
{
	debug("Starting...");

	mFormat = "particlesystem";
}

ParticleSystemManager::~ParticleSystemManager()
{
	shutdown();
}

void ParticleSystemManager::clear()
{
	debug("Clearing...");

	for ( ParticleSystemList::iterator it = mParticleSystems.begin(); it != mParticleSystems.end(); it++ ) {
		delete (*it);
	}

	mParticleSystems.clear();
}

void ParticleSystemManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mMeshManager);

	mConfigurationComplete = true;
}

void ParticleSystemManager::connectMeshManager(IObjectManager *m)
{
	mMeshManager = m;
}

Core::RenderObject* ParticleSystemManager::create(const std::string& filename)
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

	Core::Configuration::Configurator config(this, filename);
	if ( !config.success() ) {
		error("could not load configuration for '" + filename + "'!");
		return 0;
	}

	ParticleSystem *ps = new ParticleSystem();

	int size = config.getValue("emitter.size").toInt();
	for ( int c = 0; c < size; ++c ) {
		bool enabled = config.getValue("emitter", c, "enabled").toBool();
		std::string mesh = config.getValue("emitter", c, "mesh").toString();
		vector3f position = config.getValue("emitter", c, "position").toVector3f();

		Core::RenderObject *ro = mMeshManager->create(mesh);
		if ( !ro ) {
			warn("invalid <Core::RenderObject>: '" + mesh + "'");
			continue;
		}

		ParticleSystem::Emitter e(mesh);
		e.enabled(enabled);
		e.mesh(ro);
		e.position(position);

		ps->addEmitter(e);
	}

	mParticleSystems.push_back(ps);
	return ps;
}

Core::RenderObject* ParticleSystemManager::find(const std::string& name)
{
	if ( name.empty() ) {
		return 0;
	}

	for ( ParticleSystemList::iterator it = mParticleSystems.begin(); it != mParticleSystems.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), name) ) {
			return (*it);
		}
	}

	return 0;
}

void ParticleSystemManager::update(float elapsedTime)
{
	for ( ParticleSystemList::iterator it = mParticleSystems.begin(); it != mParticleSystems.end(); it++ ) {
		(*it)->update(elapsedTime);
	}
}



}
