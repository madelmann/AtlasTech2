
// Header
#include "Frustum.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Math/Tools.h>

// Namespace declarations


#define RIGHT	0	// The RIGHT side of the frustum
#define LEFT	1	// The LEFT	 side of the frustum
#define BOTTOM	2	// The BOTTOM side of the frustum
#define TOP		3	// The TOP side of the frustum
#define BACK	4	// The BACK	side of the frustum
#define FRONT	5	// The FRONT side of the frustum


Frustum::Frustum()
: mFarPlane(0.f),
  mFOV(0.f),
  mNearPlane(0.f),
  mRatio(0.f)
{
}

bool Frustum::boxInFrustum(IVolume *v) const
{
	int p = 0;
	vector3f pos = v->position();
	vector3f s = v->size();

	for ( int i = 0; i < 6; i += 1 ) {
		if ( pointInFrustum(vector3f(pos.x, pos.y, pos.z)) ) {
			p += 1;
		}
		if ( pointInFrustum(vector3f(pos.x + s.x, pos.y, pos.z)) ) {
			p += 1;
		}
		if ( pointInFrustum(vector3f(pos.x + s.x, pos.y, pos.z + s.z)) ) {
			p += 1;
		}
		if ( pointInFrustum(vector3f(pos.x, pos.y, pos.z + s.z)) ) {
			p += 1;
		}
		if ( pointInFrustum(vector3f(pos.x, pos.y + s.y, pos.z)) ) {
			p += 1;
		}
		if ( pointInFrustum(vector3f(pos.x + s.x, pos.y + s.y, pos.z)) ) {
			p += 1;
		}
		if ( pointInFrustum(vector3f(pos.x + s.x, pos.y + s.y, pos.z + s.z)) ) {
			p += 1;
		}
		if ( pointInFrustum(vector3f(pos.x, pos.y + s.y, pos.z + s.z)) ) {
			p += 1;
		}
	}

	return ( p > 0 );
}

bool Frustum::cylinderInFrustum(IVolume *v) const
{
	vector3f center = v->center();
	vector3f size = v->size();
	int o = 0;

	// Go through all the sides of the frustum
	for ( int i = 5; i >= 0; i -= 1 ) {
		// If the center of the sphere is farther away from the plane than the radius
		if ( mPlanes[i].a * center.x + mPlanes[i].b * center.y + mPlanes[i].c * center.z + mPlanes[i].d <= -size.x ) {
			// The distance was greater than the radius so the cylinder is outside of the frustum
			//return false;
			o += 1;
		}
	}

	if ( o == 6 ) {
		return false;
	}

	return true;
}

/*
 *This retrieves our distance from a frustum plane
 */
float Frustum::getDistanceToPlane(int i, const vector3f& v) const
{
	return	mPlanes[i].a * v.x +
			mPlanes[i].b * v.y +
			mPlanes[i].c * v.z + 
			mPlanes[i].d;
}

inline bool Frustum::pointInFrustum(const vector3f& v) const
{
	// Go through all the sides of the frustum
	for ( int i = 5; i >= 0; i -= 1 ) {
		// Calculate the plane equation and check if the point is behind a side of the frustum
		if ( mPlanes[i].a * v.x + mPlanes[i].b * v.y + mPlanes[i].c * v.z + mPlanes[i].d <= 0 ) {
			// The point was behind a side, so it ISN'T in the frustum
			return false;
		}
	}

	// The point was inside of the frustum (In front of ALL the sides of the frustum)
	return true;
}

inline bool Frustum::sphereInFrustum(IVolume *v) const
{
	vector3f center = v->center();
	vector3f size = v->size();
	int o = 0;

	// Go through all the sides of the frustum
	for ( int i = 5; i >= 0; i -= 1 ) {
		// If the center of the sphere is farther away from the plane than the radius
		if ( mPlanes[i].a * center.x + mPlanes[i].b * center.y + mPlanes[i].c * center.z + mPlanes[i].d <= -size.x / 2 ) {
			// The distance was greater than the radius so the sphere is outside of the frustum
			//return false;
			o += 1;
		}
	}

	if ( o == 6 ) {
		return false;
	}

	return true;
}

bool Frustum::volumeInFrustum(IVolume *v)
{
	if ( !v ) {
		return false;
	}

	switch ( v->type() ) {
		case IVolume::Type::AABBox:
		case IVolume::Type::AABBoxNoTranslate:
		case IVolume::Type::Box:
			return boxInFrustum(v);
		case IVolume::Type::Cylinder:
			return cylinderInFrustum(v);
		case IVolume::Type::Sphere:
			return sphereInFrustum(v);
	}

	// if no bounding volume type is set default to true
	return true;
}

void Frustum::update()
{
	static Matrix4f proj,								// This will hold our projection matrix
					modl,								// This will hold our modelview matrix
					comboMatrix;						// This will hold the clipping planes

	FrustumPlane *p;

	// glGetFloatv() is used to extract information about our OpenGL world.
	// Below, we pass in GL_PROJECTION_MATRIX to abstract our projection matrix.
	// It then stores the matrix into an array of [16].
	glGetFloatv( GL_PROJECTION_MATRIX, proj );

	// By passing in GL_MODELVIEW_MATRIX, we can abstract our model view matrix.
	// This also stores it in an array of [16].
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	comboMatrix  = proj;
	comboMatrix *= modl;

	// Right clipping plane
	p = &mPlanes[RIGHT];
	p->a = comboMatrix[3] - comboMatrix[0];
	p->b = comboMatrix[7] - comboMatrix[4];
	p->c = comboMatrix[11] - comboMatrix[8];
	p->d = comboMatrix[15] - comboMatrix[12];
	// Left clipping plane
	p = &mPlanes[LEFT];
	p->a = comboMatrix[3] + comboMatrix[0];
	p->b = comboMatrix[7] + comboMatrix[4];
	p->c = comboMatrix[11] + comboMatrix[8];
	p->d = comboMatrix[15] + comboMatrix[12];
	// Bottom clipping plane
	p = &mPlanes[BOTTOM];
	p->a = comboMatrix[3] + comboMatrix[1];
	p->b = comboMatrix[7] + comboMatrix[5];
	p->c = comboMatrix[11] + comboMatrix[9];
	p->d = comboMatrix[15] + comboMatrix[13];
	// Top clipping plane
	p = &mPlanes[TOP];
	p->a = comboMatrix[3] - comboMatrix[1];
	p->b = comboMatrix[7] - comboMatrix[5];
	p->c = comboMatrix[11] - comboMatrix[9];
	p->d = comboMatrix[15] - comboMatrix[13];
	// Far clipping plane
	p = &mPlanes[BACK];
	p->a = comboMatrix[3] - comboMatrix[2];
	p->b = comboMatrix[7] - comboMatrix[6];
	p->c = comboMatrix[11] - comboMatrix[10];
	p->d = comboMatrix[15] - comboMatrix[14];
	// Near clipping plane
	p = &mPlanes[FRONT];
	p->a = comboMatrix[3] + comboMatrix[2];
	p->b = comboMatrix[7] + comboMatrix[6];
	p->c = comboMatrix[11] + comboMatrix[10];
	p->d = comboMatrix[15] + comboMatrix[14];

	float magnitude = 0.0f;
	for ( int i = 5; i >= 0; i -= 1 ) {
		magnitude = fastSquareRoot(	mPlanes[i].a * mPlanes[i].a + 
									mPlanes[i].b * mPlanes[i].b + 
									mPlanes[i].c * mPlanes[i].c );

		if ( magnitude ) {
			mPlanes[i].a /= magnitude;
			mPlanes[i].b /= magnitude;
			mPlanes[i].c /= magnitude;
			mPlanes[i].d /= magnitude;
		}
	}
}