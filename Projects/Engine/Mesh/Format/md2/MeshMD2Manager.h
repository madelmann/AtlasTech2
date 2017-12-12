
#ifndef Mesh_Format_MD2_MeshMD2Manager_h
#define Mesh_Format_MD2_MeshMD2Manager_h


// Library includes
#include <list>

// Project includes
#include <Mesh/AMeshManager.h>

// Forward declarations

// Namespace declarations


namespace Mesh {

// Forward declarations
class MeshMD2;

class MeshMD2Manager : public AMeshManager
{
public:
	MeshMD2Manager(const Common::Logger *p);
	~MeshMD2Manager();

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
	typedef std::list<MeshMD2*> MeshMD2List;

private:
	MeshMD2List	mMeshes;
};


}


#endif
