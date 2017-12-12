
// Header
#include "Viewer.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Math/Tools.h>

// Namespace declarations


namespace CameraSpace {


Viewer::Viewer()
: mFOV(50.f),
  mPosition(vector3f(0.f, 0.f, 0.f)),
  mRotation(vector3f(0.f, 0.f, 0.f)),
  mView(vector3f(0.f, 0.f, 1.f)),
  mUpVector(vector3f(0.f, 1.f, 0.f))
{
}

Viewer::~Viewer()
{
}

void Viewer::convertRotationToView()
{
	vector3f view;
	view.x = (float)sin(mRotation.y * PI / 180.f);
	//view.y = (float)sin(sqrt(mRotation.x * mRotation.x + mRotation.z * mRotation.z)) * PI / 180.f;
	view.z = (float)cos(mRotation.y * PI / 180.f);
}

void Viewer::convertViewToRotation()
{
	// Get the view vector (The direction we are facing)
	vector3f vView = mView - mPosition;		

	mRotation.x = acos(vView.x / magnitude(vView)) * PI / 180.f;
	mRotation.y = acos(vView.y / magnitude(vView)) * PI / 180.f;
	mRotation.z = acos(vView.z / magnitude(vView)) * PI / 180.f;
}

const float& Viewer::fov() const
{
	return mFOV;
}

void Viewer::fov(const float& fov)
{
	mFOV = fov;
}

void Viewer::look()
{
	gluLookAt(mPosition.x, mPosition.y, mPosition.z,
			  mView.x, mView.y, mView.z,
			  mUpVector.x, mUpVector.y, mUpVector.z);
}

const vector3f& Viewer::position() const
{
	return mPosition;
}

void Viewer::position(const vector3f& pos)
{
	mPosition = pos;
}

const vector3f& Viewer::rotation() const
{
	return mRotation;
}

void Viewer::rotation(const vector3f& rotation)
{
	mRotation = rotation;
}

const vector3f& Viewer::view() const
{
	return mView;
}

void Viewer::view(const vector3f& view)
{
	mView = view;
}

void Viewer::update(float /*elapsedTime*/)
{
	// nothing to do here
}

const vector3f& Viewer::upVector() const
{
	return mUpVector;
}

void Viewer::upVector(const vector3f& up)
{
	mUpVector = up;
}


}
