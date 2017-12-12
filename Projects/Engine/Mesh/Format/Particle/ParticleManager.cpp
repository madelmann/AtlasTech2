
// Header
#include "ParticleManager.h"

// Library includes

// Project includes
#include "Particle.h"
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


ParticleManager::ParticleManager(const Common::Logger *p)
: AMeshManager(p, "ParticleManager"),
  mMeshManager(0)
{
	debug("Starting...");

	mFormat = "particle";
}

ParticleManager::~ParticleManager()
{
	shutdown();
}

void ParticleManager::clear()
{
	debug("Clearing...");

	for ( ParticleList::iterator it = mParticles.begin(); it != mParticles.end(); it++ ) {
		delete (*it);
	}

	mParticles.clear();
}

void ParticleManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mMeshManager);

	mConfigurationComplete = true;
}

void ParticleManager::connectMeshManager(IObjectManager *m)
{
	mMeshManager = m;
}

Core::RenderObject* ParticleManager::create(const std::string& filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
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


	Particle *particle = new Particle();

	{	// mesh setup
		//particle->name(meshName);
		particle->setAlphaValue(config.getValue("alpha").toFloat());
		particle->setAttenuation(config.getValue("attenuation").toVector3f());
		particle->setAirResistence(config.getValue("airresistance").toBool());
		particle->setBlending(config.getValue("blending").toBool());
		particle->setColor(config.getValue("color").toVector3f());
		particle->setFadeInTime(config.getValue("fadein").toFloat());
		particle->setFadeOutTime(config.getValue("fadeout").toFloat());
		particle->setGlobalGravity(config.getValue("globalgravity").toBool());
		particle->setGravity(config.getValue("gravity").toVector3f());
		particle->setLifeTime(config.getValue("lifetime").toFloat());
		particle->setLoopExecution(config.getValue("loopexecution").toBool());
		particle->setMaxParticles(config.getValue("maxparticles").toInt());
		particle->setParticleSize(config.getValue("size").toFloat());
		particle->setPolygonalMode(config.getValue("polygonmode").toBool());
		particle->setNumToRelease(config.getValue("numtorelease").toInt());
		particle->setRange(config.getValue("range").toVector3f());
		particle->setReleaseInterval(config.getValue("releaseinterval").toFloat());
		particle->setVelocity(config.getValue("velocity").toVector3f());
		particle->setVariableVelocity(config.getValue("velocityvariable").toVector3f());

		std::string renderObject = config.getValue("mesh").toString();
		if ( !renderObject.empty() ) {
			if ( renderObject == meshName ) {
				fatal("recursive mesh set: '" + renderObject + "'!");
			}

			Core::RenderObject *ro = mMeshManager->create(renderObject + ".ao");
			particle->setMesh(ro);
		}

		particle->init();
	}

	mParticles.push_back(particle);
	return particle;
}

Core::RenderObject* ParticleManager::find(const std::string& name)
{
	if ( name.empty() ) {
		return 0;
	}

	for ( ParticleList::iterator it = mParticles.begin(); it != mParticles.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), name) ) {
			return (*it);
		}
	}

	return 0;
}

void ParticleManager::update(float elapsedTime)
{
	for ( ParticleList::iterator it = mParticles.begin(); it != mParticles.end(); it++ ) {
		(*it)->update(elapsedTime);
	}
}


}
