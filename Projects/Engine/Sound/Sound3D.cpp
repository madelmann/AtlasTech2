
// Header
#include "Sound3D.h"

// Libary includes

// Project includes

// Namespace declarations


namespace SoundSpace {


Sound3D::Sound3D()
: mPositionZ(0.f)
{
}

Sound3D::~Sound3D()
{
}

const vector3f Sound3D::position() const
{
	return vector3f(mPositionX, mPositionY, mPositionZ);
}

void Sound3D::position(const vector3f& position)
{
	mPositionX = position.x;
	mPositionY = position.y;
	mPositionZ = position.z;

	if ( mResource ) {
		alSource3f(mResource->data().mALSource, AL_POSITION, mPositionX, mPositionY, mPositionZ);
	}
}


}
