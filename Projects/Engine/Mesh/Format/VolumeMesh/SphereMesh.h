
#ifndef Mesh_Format_VolumeMesh_SphereMesh_h
#define Mesh_Format_VolumeMesh_SphereMesh_h


// Library includes

// Project includes
#include "AVolumeMesh.h"

// Forward declarations

// Namespace declarations


namespace Mesh {


class SphereMesh : public AVolumeMesh
{
public:
	SphereMesh(const vector3f& size)
	: AVolumeMesh(size)
	{ }

public:
	void init() {
		aeDelete( mBoundingVolume );
		mBoundingVolume = new Volume::SphereVolume(mSize);
	}
};


}


#endif
