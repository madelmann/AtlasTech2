
#ifndef _Mesh_Volume_Sphere_h_
#define _Mesh_Volume_Sphere_h_


// Library includes

// Project includes
#include <Interfaces/Mesh/IVolume.h>

// Forward declarations

// Namespace declarations


namespace Mesh {
namespace Volume {


class SphereVolume : public IVolume
{
public:
	SphereVolume(const vector3f& size = vector3f());

	void render(bool solid = false);

	vector3f size();

protected:

private:

};


}
}


#endif
