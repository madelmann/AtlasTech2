
// Header
#include "Mesh3DSManager.h"

// Library includes

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Tools/Tools.h>
#include "3dsloader.h"
#include "Mesh3DS.h"

// Namespace declarations


namespace Mesh {


Mesh3DSManager::Mesh3DSManager(const Common::Logger *p)
: AMeshManager(p, "Mesh3DSManager")
{
	debug("Starting...");

	mFormat = "3ds";
}

Mesh3DSManager::~Mesh3DSManager()
{
	shutdown();
}

void Mesh3DSManager::clear()
{
	debug("Clearing...");

	for ( Mesh3DSList::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it ) {
		delete (*it);
	}

	mMeshes.clear();
}

void Mesh3DSManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	mConfigurationComplete = true;
}

Core::RenderObject* Mesh3DSManager::create(const std::string& filename)
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

	Core::Configuration::Configurator config(this, Tools::Files::ChangeFileExt(filename, "conf"));
	if ( !config.success() ) {
		error("could not load configuration for '" + filename + "'!");
		//return 0;
	}

	Mesh3DS *mesh = new Mesh3DS();

	{	// mesh setup
		mesh3ds_vbo mesh_vbo;
		if ( !load3DS(&mesh_vbo, filename) ) {
			delete mesh;

			return 0;
		}

		//mesh->name(meshName);
		mesh->setMesh(mesh_vbo);
	}

	mMeshes.push_back(mesh);
	return mesh;
}

Core::RenderObject* Mesh3DSManager::find(const std::string& filename)
{
	if ( filename.empty() ) {
		return 0;
	}

	for ( Mesh3DSList::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it ) {
		if ( Tools::StringCompare((*it)->name(), filename) ) {
			return (*it);
		}
	}

	return 0;
}


}
