
#ifndef _Frustum_Frustum_h_
#define _Frustum_Frustum_h_


// Library includes

// Project includes
#include <Interfaces/Mesh/IVolume.h>
#include <Math/Matrix4.h>
#include <Math/Tuple2.h>
#include <Math/Tuple3.h>
#include <Math/Tuple4.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


struct FrustumPlane
{
	FrustumPlane()
	: a(0.f),
	  b(0.f),
	  c(0.f),
	  d(0.f)
	{ }

	float a, b, c, d;
};

class Frustum
{
public:
	Frustum();

public:
	bool boxInFrustum(IVolume *v) const;
	bool cylinderInFrustum(IVolume *v) const;
    float getDistanceToPlane(int plane, const vector3f& v) const;
    inline bool pointInFrustum(const vector3f& v) const;
	bool sphereInFrustum(IVolume *v) const;
	bool volumeInFrustum(IVolume *v);

	void update();

public:
	float			mFarPlane;
	float			mFOV;
	float			mNearPlane;
	FrustumPlane	mPlanes[6];
	float			mRatio;

protected:

private:

};


#endif
