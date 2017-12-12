
// Header
#include "MeshOBJManager.h"

// Library includes

// Project includes
#include "MeshOBJ.h"
#include "objloader.h"
#include <Core/Configuration/Configurator.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


MeshOBJManager::MeshOBJManager(const Common::Logger *p)
: AMeshManager(p, "MeshOBJManager")
{
	debug("Starting...");

	mFormat = "obj";
}

MeshOBJManager::~MeshOBJManager()
{
	shutdown();
}

void MeshOBJManager::clear()
{
	debug("Clearing...");

	for ( MeshOBJList::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		delete (*it);
	}

	mMeshes.clear();
}

void MeshOBJManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	mConfigurationComplete = true;
}

Core::RenderObject* MeshOBJManager::create(const std::string& filename)
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

	MeshOBJ *mesh = new MeshOBJ();

	{	// mesh setup
		obj_t mesh_vbo;
		if ( !loadOBJ(&mesh_vbo, filename) ) {
			error("could not load mesh from '" + filename + "'!");
			delete mesh;

			return 0;
		}

		mesh->setMesh(mesh_vbo);
	}

	mMeshes.push_back(mesh);
	return mesh;
}

Core::RenderObject* MeshOBJManager::find(const std::string& filename)
{
	if ( filename.empty() ) {
		return 0;
	}

	for ( MeshOBJList::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), filename) ) {
			return (*it);
		}
	}

	return 0;
}


}
