
// Header
#include "MeshMD2Manager.h"

// Library includes

// Poject includes
#include "Md2.h"
#include "MeshMD2.h"
#include <Core/Configuration/Configurator.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


MeshMD2Manager::MeshMD2Manager(const Common::Logger *p)
: AMeshManager(p, "MeshMD2Manager")
{
	debug("Starting...");

	mFormat = "md2";
}

MeshMD2Manager::~MeshMD2Manager()
{
	shutdown();
}

void MeshMD2Manager::clear()
{
	debug("Clearing...");

	for ( MeshMD2List::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		delete (*it);
		(*it) = 0;				// in case this pointer is used anywhere else
	}

	mMeshes.clear();
}

void MeshMD2Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	mConfigurationComplete = true;
}

Core::RenderObject* MeshMD2Manager::create(const std::string& filename)
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

	char fname[255] = "";
	sprintf(fname, "%s", filename.c_str());

	MeshMD2 *mesh = new MeshMD2();

	{	// mesh setup
		CLoadMD2 md2Loader(this);
		t3DModel *model = md2Loader.ImportMD2(fname, 0);

		//mesh->name(meshName);
		mesh->setModel(model);
	}

	mMeshes.push_back(mesh);
	return mesh;
}

Core::RenderObject* MeshMD2Manager::find(const std::string& filename)
{
	if ( filename.empty() ) {
		return 0;
	}

	for ( MeshMD2List::iterator it = mMeshes.begin(); it != mMeshes.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), filename) ) {
			return (*it);
		}
	}

	return 0;
}


}
