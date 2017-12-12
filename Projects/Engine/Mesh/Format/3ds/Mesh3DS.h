
#ifndef Mesh_Format_3ds_Mesh3DS_h
#define Mesh_Format_3ds_Mesh3DS_h


// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Mesh/AMesh.h>
#include "Types.h"

// Forward declarations

// Namespace declarations


namespace Mesh {


class Mesh3DS : public AMesh
{
public:
	Mesh3DS(Core::AEid id = Core::invalidID, const std::string& name = "");
	virtual ~Mesh3DS();

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

	void setMesh(const mesh3ds_vbo& mesh);

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
	mesh3ds_vbo	mMesh;
};


}


#endif
