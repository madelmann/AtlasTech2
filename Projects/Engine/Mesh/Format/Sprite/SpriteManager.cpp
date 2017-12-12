
// Header
#include "SpriteManager.h"

// Library includes

// Project includes
#include "Sprite.h"
#include <Core/Configuration/Configurator.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


SpriteManager::SpriteManager(const Common::Logger *p)
: AMeshManager(p, "SpriteManager")
{
	debug("Starting...");

	mFormat = "sprite";
}

SpriteManager::~SpriteManager()
{
	shutdown();
}

void SpriteManager::clear()
{
	debug("Clearing...");

	for ( SpriteList::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		delete (*it);
	}

	mMeshes.clear();
}

void SpriteManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	mConfigurationComplete = true;
}

Core::RenderObject* SpriteManager::create(const std::string& filename)
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

	//Core::Configuration::Configurator config(this, Tools::Files::ChangeFileExt(filename, "conf"));
	Core::Configuration::Configurator config(this, filename);
	if ( !config.success() ) {
		warn("could not load configuration for '" + filename + "'!");
		//return 0;
	}

	Sprite *mesh = new Sprite();

	{	// mesh setup
		mesh->setTextureCoordAnim(config.getValue("texturecoordanim").toVector2f());
		mesh->setTextureCoords(config.getValue("texturecoords").toVector2f());
	}

	mMeshes.push_back(mesh);
	return mesh;
}

Core::RenderObject* SpriteManager::find(const std::string& filename)
{
	if ( filename.empty() ) {
		return 0;
	}

	for ( SpriteList::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), filename) ) {
			return (*it);
		}
	}

	return 0;
}


}
