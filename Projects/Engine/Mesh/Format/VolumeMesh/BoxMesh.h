
#ifndef Mesh_Format_VolumeMesh_BoxMesh_h
#define Mesh_Format_VolumeMesh_BoxMesh_h


// Library includes

// Project includes
#include "AVolumeMesh.h"

// Forward declarations

// Namespace declarations


namespace Mesh {


class BoxMesh : public AVolumeMesh
{
public:
	BoxMesh(const vector3f& size)
	: AVolumeMesh(size)
	{ }

public:
	void init() {
		aeDelete( mBoundingVolume );
		mBoundingVolume = new Volume::AABBoxVolume(mSize);
	}
};


}


#endif
