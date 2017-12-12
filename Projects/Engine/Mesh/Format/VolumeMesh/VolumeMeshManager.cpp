
// Header
#include "VolumeMeshManager.h"

// Library includes

// Project includes
#include "BoxMesh.h"
#include "SphereMesh.h"
#include <Core/Configuration/Configurator.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


VolumeMeshManager::VolumeMeshManager(const Common::Logger *p)
: AMeshManager(p, "VolumeMeshManager")
{
	debug("Starting...");

	mFormat = "BoxMesh";
}

VolumeMeshManager::~VolumeMeshManager()
{
	shutdown();
}

void VolumeMeshManager::clear()
{
	debug("Clearing...");

	for ( VolumeMeshList::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		delete (*it);
	}

	mMeshes.clear();
}

void VolumeMeshManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	mConfigurationComplete = true;
}

Core::RenderObject* VolumeMeshManager::create(const std::string& filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("create(): invalid filename provided!");
		return 0;
	}

	std::string meshName = Tools::Files::ExtractFileExt(filename);

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

	AVolumeMesh *mesh = 0;

	{	// mesh setup
		static vector3f size(1.f, 1.f, 1.f);

		if ( meshName == "BoxMesh" ) {
			mesh = new BoxMesh(size);
		}
		else if ( meshName == "SphereMesh" ) {
			mesh = new SphereMesh(size);
		}
		else {
			assert(!"invalid mesh volume specified!");
		}

		//mesh->name(meshName);
	}

	mMeshes.push_back(mesh);
	return mesh;
}

Core::RenderObject* VolumeMeshManager::find(const std::string& filename)
{
	if ( filename.empty() ) {
		return 0;
	}

	for ( VolumeMeshList::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), filename) ) {
			return (*it);
		}
	}

	return 0;
}


}
