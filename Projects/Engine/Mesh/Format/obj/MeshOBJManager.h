
#ifndef Mesh_Format_obj_MeshOBJManager_h
#define Mesh_Format_obj_MeshOBJManager_h


// Library includes

// Project includes
#include <Mesh/AMeshManager.h>

// Forward declarations

// Namespace declarations


namespace Mesh {

// Forward declarations
class MeshOBJ;

class MeshOBJManager : public AMeshManager
{
public:
	MeshOBJManager(const Common::Logger *p);
	virtual ~MeshOBJManager();

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
	typedef std::list<MeshOBJ*> MeshOBJList;

private:
	MeshOBJList	mMeshes;
};


}


#endif
