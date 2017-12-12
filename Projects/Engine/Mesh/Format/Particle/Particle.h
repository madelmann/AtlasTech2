
#ifndef Mesh_Format_Particle_Particle_h
#define Mesh_Format_Particle_Particle_h


// Library includes

// Project includes
#include "Types.h"
#include <Math/Vector3.h>
#include <Mesh/AMesh.h>

// Forward declarations

// Namespace declarations


namespace Mesh {


//-----------------------------------------------------------------------------
// SYMBOLIC CONSTANTS
//-----------------------------------------------------------------------------

// Classify Point
const int CP_FRONT   = 0;
const int CP_BACK    = 1;
const int CP_ONPLANE = 2;

// Collision Results
const int CR_BOUNCE  = 0;
const int CR_STICK   = 1;
const int CR_RECYCLE = 2;

const float OGL_PI = 3.141592654f;


struct Plane_s
{
    vector3f	mNormal;			// The plane's normal
    vector3f	mPoint;				// A coplanar point within the plane
    float		mBounceFactor;		// Coefficient of restitution (or how bouncy the plane is)
    int			mCollisionResult;	// What will particles do when they strike the plane

    Plane_s		*mNext;				// Next plane in list
};

struct Particle_s
{
	float		mAlpha;
    vector3f	mCurPos;			// Current position of particle
    vector3f	mCurVel;			// Current velocity of particle
    float		mInitTime;			// Time of creation of particle

    Particle_s	*mNext;				// Next particle in list
};

struct PointVertex_s
{
    vector3f	posit;
    vector3f	color;
};



class Particle : public AMesh
{
public:
	Particle(Core::AEid id = Core::invalidID, const std::string& name = "");
	virtual ~Particle();

public:
	// Core::RenderObject implementation
	//{
	void init();
	void render();
	void reset();
	void update(float elpasedTime = 0.f);
	//}

	// Control
	// {
	void pause();
	void resume();
	// }

	void setAirResistence(bool airResistence);
	void setAlphaValue(float alpha);
	void setAttenuation(const vector3f& attenuation);
	void setBlending(bool state);
	void setCollisionPlane(const vector3f& vPlaneNormal, const vector3f& vPoint, 
						   float fBounceFactor = 1.0f, int nCollisionResult = CR_BOUNCE);
	void setColor(const vector3f& color);
	void setFadeInTime(float time);
	void setFadeOutTime(float time);
	void setGlobalGravity(bool state);
	void setGravity(const vector3f& gravity);
	void setLifeTime(float time);
	void setLoopExecution(bool state);
	void setMaxParticles(int max);
	void setMesh(Core::RenderObject *ro);
	void setNumToRelease(int release);
	void setParticleSize(float size);
	void setPolygonalMode(bool state);
	void setRange(const vector3f& range);
	void setReleaseInterval(float interval);
	void setVariableVelocity(const vector3f& variable);
	void setVelocity(const vector3f& velocity);
	void setVolumeType(IVolume::Type::E type);

protected:

private:
	// Core::RenderObject implementation
	//{
	void setBoundingVolume();
	void unload();
	//}

private:
	bool		mAirResistence;
	float		mAlpha;
	bool		mApplyGlobalGravity;
	vector3f	mAttenuation;
	bool		mBlending;
	vector3f	mColor;
	float		mFadeInTime;
	float		mFadeOutTime;
	vector3f	mGravity;
	float		mLifeTime;
	bool		mLoopExecution;
	int			mMaxParticles;
	Core::RenderObject	*mMesh;
	int			mNumToRelease;
	float		mParticleSize;
	bool		mPause;
	bool		mPolygonalMode;
	vector3f	mRange;
	float		mReleaseInterval;
	vector3f	mVariableVelocity;
	vector3f	mVelocity;
	IVolume::Type::E	mVolumeType;

	Particle_s	*mActiveList;
	Particle_s	*mFreeList;
	Plane_s		*mPlanes;
	int			mActiveCount;
	float		mCurrentTime;
	float		mLastUpdate;
	float		mMaxPointSize;
	float		mMinPointSize;
	bool		mDeviceSupportsPSIZE;

	ColorBuffer		mColorBuffer;
	VertexBuffer	mVertexBuffer;
};


}


#endif
