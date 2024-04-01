
// Header
#include "Mouse.h"

// Library includes

// Project includes

// Namespace declarations


namespace Input {


Mouse::Mouse()
{
	// nothing to do here
}

Mouse::~Mouse()
{
	// nothing to do here
}

const vector2l& Mouse::getPosition2D() const
{
	return mPosition2D;
}

const vector3f& Mouse::getPosition3D() const
{
	return mPosition3D;
}

void Mouse::setPosition2D(const vector2l& pos)
{
	mPosition2D = pos;
}

void Mouse::setPosition3D(const vector3f& pos)
{
	mPosition3D = pos;
}


}
