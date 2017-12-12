
#ifndef Mesh_Format_MD5Mesh_MeshMD5Manager_h
#define Mesh_Format_MD5Mesh_MeshMD5Manager_h


// Library includes
#include <list>

// Project includes
#include <Mesh/AMeshManager.h>

// Forward declarations
class MD5Model;

// Namespace declarations


namespace Mesh {

// Forward declarations
class MeshMD5;

class MeshMD5Manager : public AMeshManager
{
public:
	MeshMD5Manager(const Common::Logger *p);
	~MeshMD5Manager();

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
	typedef std::list<MeshMD5*> MeshMD5List;

private:
	void loadAnimations(MD5Model *model, const std::string& path);

private:
	MeshMD5List	mMeshes;
};


}


#endif
