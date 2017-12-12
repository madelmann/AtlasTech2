
// Header
#include "Sound2D.h"

// Library includes

// Project includes

// Namespace declarations


namespace SoundSpace {


Sound2D::Sound2D()
: mPositionX(0.f),
  mPositionY(0.f)
{
}

Sound2D::~Sound2D()
{
}

const vector2f Sound2D::position() const
{
	return vector2f(mPositionX, mPositionY);
}

void Sound2D::position(const vector2f& position)
{
	mPositionX = position.x;
	mPositionY = position.y;

	if ( mResource ) {
		alSourcei(mResource->data().mALSource, AL_SOURCE_RELATIVE, AL_TRUE);
	}
}


}
