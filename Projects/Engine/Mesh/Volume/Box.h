
#ifndef _Mesh_Volume_Box_h_
#define _Mesh_Volume_Box_h_


// Library includes

// Project includes
#include <Interfaces/Mesh/IVolume.h>

// Forward declarations

// Namespace declarations


namespace Mesh {
namespace Volume {


class BoxVolume : public IVolume
{
public:
	BoxVolume(const vector3f& size = vector3f());

public:
	void render(bool solid = false);

protected:

private:

};


}
}


#endif
