
// Header
#include "MeshMD5Manager.h"

// Library includes

// Poject includes
#include "MD5Model.h"
#include "MeshMD5.h"
#include <Core/Configuration/Configurator.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


MeshMD5Manager::MeshMD5Manager(const Common::Logger *p)
: AMeshManager(p, "MeshMD5Manager")
{
	debug("Starting...");

	mFormat = "md5mesh";
}

MeshMD5Manager::~MeshMD5Manager()
{
	shutdown();
}

void MeshMD5Manager::clear()
{
	debug("Clearing...");

	for ( MeshMD5List::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		aeDelete( (*it) );
	}

	mMeshes.clear();
}

void MeshMD5Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	mConfigurationComplete = true;
}

Core::RenderObject* MeshMD5Manager::create(const std::string& filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("create(): invalid filename provided!");
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

	MeshMD5 *mesh = new MeshMD5();

	{	// mesh setup
		MD5Model *md5model = new MD5Model();
		md5model->loadMesh(filename.c_str());

		loadAnimations(md5model, Tools::Files::ExtractPathname(filename));

		mesh->setModel(md5model);
	}

	mMeshes.push_back(mesh);
	return mesh;
}

Core::RenderObject* MeshMD5Manager::find(const std::string& filename)
{
	if ( filename.empty() ) {
		return 0;
	}

	for ( MeshMD5List::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), filename) ) {
			return (*it);
		}
	}

	return 0;
}

void MeshMD5Manager::loadAnimations(MD5Model *model, const std::string& path)
{
	if ( !model ) {
		error("invalid MD5Model provided!");
		return;
	}

	model->loadAnim(std::string(path + "idle01.md5anim").c_str());
}


}
