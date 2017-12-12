
#ifndef Mesh_Format_VolumeMesh_AVolumeMesh_h
#define Mesh_Format_VolumeMesh_AVolumeMesh_h


// Library includes

// Project includes
#include <Math/Vector3.h>
#include <Mesh/AMesh.h>
#include <Mesh/Volume/Volumes.h>
#include <Tools/Tools.h>

// Forward declarations

// Namespace declarations


namespace Mesh {


class AVolumeMesh : public AMesh
{
public:
	AVolumeMesh(const vector3f& size);
	virtual ~AVolumeMesh();

public:
	// Core::RenderObject implementation
	//{
	void render();
	void update(float elapsedTime);
	//}

	// AMesh implementation
	// {
	void setAnimation(const std::string& animation);
	// }

	void resize(const vector3f& size);

protected:
	// Core::RenderObject implementation
	//{
	void unload();
	//}

private:

};


}


#endif
