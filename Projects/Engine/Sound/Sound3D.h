
#ifndef _Sound_Sound3D_h_
#define _Sound_Sound3D_h


// Library includes

// Project includes
#include "Sound2D.h"
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace SoundSpace {


class Sound3D : public Sound2D
{
public:
	Sound3D();
	virtual ~Sound3D();

	const vector3f position() const;
	void position(const vector3f& position);

protected:
	float	mPositionZ;

private:

};


}


#endif
