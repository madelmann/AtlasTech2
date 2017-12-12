
// Header
#include "External.h"

// Library includes
#include <math.h>

// Project includes
#include <Driver/GLWrapper.h>
#include <Math/Tools.h>

// Namespace declarations


void External::look()
{
	gluLookAt(mPosition.x + mOffset.x, mPosition.y + mOffset.y, mPosition.z + mOffset.z,
			  mView.x + mOffset.x, mView.y + mOffset.y, mView.z + mOffset.z,
			  mUpVector.x, mUpVector.y, mUpVector.z);
}

void External::offset(const vector3f& offset)
{
	mOffset = offset;
}

void External::rotation(const vector3f& rotation)
{
	mRotation = rotation;

	mView.x = mPosition.x + (float)sin(mRotation.y * PI / 180);
	//mView.y = mPosition.y;
	mView.z = mPosition.z + (float)cos(mRotation.y * PI / 180);
}
