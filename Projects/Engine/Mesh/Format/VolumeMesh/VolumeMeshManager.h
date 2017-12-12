
#ifndef Mesh_Format_VolumeMesh_VolumeMeshManager_h
#define Mesh_Format_VolumeMesh_VolumeMeshManager_h


// Library includes
#include <list>

// Project includes
#include <Mesh/AMeshManager.h>

// Forward declarations

// Namespace declarations


namespace Mesh {

// Forward declarations
class AVolumeMesh;

class VolumeMeshManager : public AMeshManager
{
public:
	VolumeMeshManager(const Common::Logger *p);
	~VolumeMeshManager();

public:
	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	//}

	// IMeshManager implementation
	//{
	Core::RenderObject* create(const std::string& filename);
	Core::RenderObject* find(const std::string& filename);
	//}

protected:

private:
	typedef std::list<AVolumeMesh*> VolumeMeshList;

private:
	VolumeMeshList	mMeshes;
};


}


#endif
