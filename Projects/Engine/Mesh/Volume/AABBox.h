
#ifndef _Mesh_Volume_AABBox_h_
#define _Mesh_Volume_AABBox_h_


// Library includes

// Project includes
#include <Interfaces/Mesh/IVolume.h>

// Forward declarations

// Namespace declarations


namespace Mesh {
namespace Volume {


class AABBoxVolume : public IVolume
{
public:
	AABBoxVolume(const vector3f& size = vector3f());

	void render(bool solid = false);

protected:

private:

};


}
}


#endif
