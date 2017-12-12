
#ifndef Mesh_Format_obj_MeshOBJ_h
#define Mesh_Format_obj_MeshOBJ_h


// Library includes

// Project includes
#include "Types.h"
#include <Mesh/AMesh.h>

// Forward declarations

// Namespace declarations


namespace Mesh {


class MeshOBJ : public AMesh
{
public:
	MeshOBJ(Core::AEid id = Core::invalidID, const std::string& name = "");
	virtual ~MeshOBJ();

public:
	// Core::RenderObject implementation
	//{
	void init();
	void render();
	//}

	// AMesh implementation
	// {
	void setAnimation(const std::string& animation);
	// }

	void setMesh(obj_t mesh);

protected:

private:
	// Core::RenderObject implementation
	//{
	void setBoundingVolume();
	void unload();
	//}

	void generateIndexBuffer();
	void generateNormals();
	void generateVertexBuffers();

	void preprocess();

private:
	obj_t	mMesh;
};


}


#endif
