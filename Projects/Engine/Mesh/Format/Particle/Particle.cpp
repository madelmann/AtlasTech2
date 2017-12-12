
// Header
#include "Particle.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Mesh/Volume/Volumes.h>
#include <Mesh/Utils.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


int RangedRandom(int range_min, int range_max)
{
   // Generate random numbers in the half-closed interval
   // [range_min, range_max). In other words,
   // range_min <= random number < range_max
   int u = int((double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min);

   return u;
}

//-----------------------------------------------------------------------------
// Name: getRandomMinMax()
// Desc: Gets a random number between min/max boundaries
//-----------------------------------------------------------------------------
float getRandomMinMax(float fMin, float fMax)
{
    float fRandNum = (float)rand () / RAND_MAX;
    return fMin + (fMax - fMin) * fRandNum;
}

//-----------------------------------------------------------------------------
// Name: getRandomVector()
// Desc: Generates a random vector where X,Y, and Z components are between
//       -1.0 and 1.0
//-----------------------------------------------------------------------------
vector3f getRandomVector()
{
	vector3f vVector;

    // Pick a random Z between -1.0f and 1.0f.
    vVector.z = getRandomMinMax( -1.0f, 1.0f );

    // Get radius of this circle
    float radius = (float)sqrt(1 - vVector.z * vVector.z);

    // Pick a random point on a circle.
    float t = getRandomMinMax( -OGL_PI, OGL_PI );

    // Compute matching X and Y for our Z.
    vVector.x = (float)cosf(t) * radius;
    vVector.y = (float)sinf(t) * radius;

	return vVector;
}

//-----------------------------------------------------------------------------
// Name : classifyPoint()
// Desc : Classifies a point against the plane passed
//-----------------------------------------------------------------------------
int classifyPoint( vector3f *vPoint, Plane_s *pPlane )
{
	vector3f vDirection = pPlane->mPoint - *vPoint;

    float fResult = vector3f::dotProduct(vDirection, pPlane->mNormal);

	if ( fResult < -0.001f ) {
        return CP_FRONT;
	}

	if ( fResult >  0.001f ) {
        return CP_BACK;
	}

	return CP_ONPLANE;
}


Particle::Particle(Core::AEid id, const std::string& name)
: AMesh(id, name),
  mActiveCount(0),
  mActiveList(0),
  mAirResistence(true),
  mAlpha(1.f),
  mApplyGlobalGravity(false),
  mAttenuation(vector3f(1.f, 0.f, 0.f)),
  mBlending(false),
  mCurrentTime(0.f),
  mFadeInTime(0.f),
  mFadeOutTime(0.f),
  mFreeList(0),
  mLastUpdate(0.f),
  mLoopExecution(false),
  mMaxParticles(0),
  mMaxPointSize(0.f),
  mMesh(0),
  mMinPointSize(0.f),
  mNumToRelease(0),
  mParticleSize(1.f),
  mPause(false),
  mPlanes(0),
  mPolygonalMode(false),
  mReleaseInterval(0.f),
  mVolumeType(IVolume::Type::AABBox)
{
}

Particle::~Particle()
{
	unload();
}

void Particle::init()
{
	aeCreateBuffer(mColorBuffer.mDriverId);
	aeCreateBuffer(mVertexBuffer.mDriverId);

	// init all particles
	for ( int i = 0; i < mMaxParticles; ++i ) {
		mColorBuffer.mData.push_back(vector4<float>());
		mVertexBuffer.mData.push_back(vector3<float>());
	}

	// Copy the host data into the vertex buffer objects
	glBindBuffer( GL_ARRAY_BUFFER, mColorBuffer.mDriverId );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector4<float>) * mColorBuffer.mData.size(), &(mColorBuffer.mData[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mVertexBuffer.mDriverId );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector3<float>) * mVertexBuffer.mData.size(), &(mVertexBuffer.mData[0]), GL_STATIC_DRAW );

	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);


	// Query for the max point size supported by the hardware
	glGetFloatv( GL_POINT_SIZE_MAX_ARB, &mMaxPointSize );
	glGetFloatv( GL_POINT_SIZE_MIN_ARB, &mMinPointSize );

	// To make sure points don't get over- or undersized
	if ( mParticleSize < mMinPointSize ) {
		mParticleSize = mMinPointSize;
	}
	if ( mParticleSize > mMaxPointSize ) {
		mParticleSize = mMaxPointSize;
	}

	// This is how will our point sprite's size will be modified by 
	// distance from the viewer.
	float quadratic[] =  { mAttenuation.x, mAttenuation.y, mAttenuation.z };
	glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic);

	// The alpha of a point is calculated to allow the fading of points 
	// instead of shrinking them past a defined threshold size. The threshold 
	// is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to 
	// the minimum and maximum point sizes.
	glPointParameterfARB(GL_POINT_FADE_THRESHOLD_SIZE_ARB, 10.0f);

	glPointParameterfARB(GL_POINT_SIZE_MIN_ARB, mMinPointSize);
	glPointParameterfARB(GL_POINT_SIZE_MAX_ARB, mMaxPointSize);

	setBoundingVolume();

	reset();
}

void Particle::pause()
{
	mPause = true;
}

void Particle::resume()
{
	mPause = false;
}

void Particle::render()
{
    Particle_s *pParticle = mActiveList;
	if ( !pParticle ) {
		// no active particles in system
		return;
	}

	aeTranslate(getTranslation());
	aeRotate(getRotation());

	aePushAttrib(GL_ALL_ATTRIB_BITS);

	applyFaceCulling(mFacing);

	aeColor(mColor);

	bool depthMask = aeGetState(GL_DEPTH_WRITEMASK);
	if ( depthMask ) {
		aeDepthMask(GL_FALSE);
	}

	bool lighting = aeGetState(GL_LIGHTING);
	if ( lighting ) {
		aeDisable(GL_LIGHTING);
	}

	aeBind(mMaterial);
	aeBind(mColorMap);

	if ( mMesh ) {
		while ( pParticle ) {
			aeColor4f(mColor.x, mColor.y, mColor.z, pParticle->mAlpha);

			aePushMatrix();
				aeTranslate(pParticle->mCurPos);

				mMesh->render();
			aePopMatrix();

			pParticle = pParticle->mNext;
		}
	}
	else {
		if ( mPolygonalMode ) {
			static float size = mParticleSize / 2.f;

			//static float cam[3];
			//cam[0] = g_Camera.Position().x;
			//cam[1] = g_Camera.Position().y;
			//cam[2] = g_Camera.Position().z;

			static float worldPos[3];

			while ( pParticle ) {
				worldPos[0] = pParticle->mCurPos.x;
				worldPos[1] = pParticle->mCurPos.y;
				worldPos[2] = pParticle->mCurPos.z;

				aeColor4f(mColor.x, mColor.y, mColor.z, pParticle->mAlpha);

				aePushMatrix();
					glTranslatef(worldPos[0], worldPos[1], worldPos[2]);

					// Render each particle with billboarding ...
					//BillBoard_Spherical_Begin(cam, worldPos);
						aeBegin(GL_TRIANGLE_STRIP);
							aeTexCoord2f(1.0f, 1.0f); aeVertex3f( size, size, 0.0f );
							aeTexCoord2f(0.0f, 1.0f); aeVertex3f( -size, size, 0.0f );
							aeTexCoord2f(1.0f, 0.0f); aeVertex3f( size, -size, 0.0f );
							aeTexCoord2f(0.0f, 0.0f); aeVertex3f( -size, -size, 0.0f );
						aeEnd();
					//BillBoard_End();
				aePopMatrix();

				pParticle = pParticle->mNext;
			}
		}
		else {
			//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			// Specify point sprite texture coordinate replacement mode for each texture unit
			glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

			glPointSize(mParticleSize);
			aeEnable(GL_POINT_SPRITE_ARB);

			aeBegin(GL_POINTS);
				// Render each particle...
				while ( pParticle ) {
					aeColor4f(mColor.x, mColor.y, mColor.z, pParticle->mAlpha);

					aeVertex3f( pParticle->mCurPos.x, pParticle->mCurPos.y, pParticle->mCurPos.z );

					pParticle = pParticle->mNext;
				}
			aeEnd();

/*
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			// bind color
			glBindBuffer( GL_ARRAY_BUFFER, mColorBuffer.mDriverId );
			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vector4<float>) * mColorBuffer.mData.size(), &(mColorBuffer.mData[0]) );
			glColorPointer(4, GL_FLOAT, 0, 0);

			// bind vertices 
			glBindBuffer( GL_ARRAY_BUFFER, mVertexBuffer.mDriverId );
			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vector3<float>) * mVertexBuffer.mData.size(), &(mVertexBuffer.mData[0]) );
			glVertexPointer( 3, GL_FLOAT, 0, BUFFER_OFFSET(0) );

			// draw the vbo
			glDisable(GL_DEPTH_TEST);

			glDrawArrays(GL_QUADS, 0, mVertexBuffer.mData.size());

			glEnable(GL_DEPTH_TEST);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
*/

			aeDisable(GL_POINT_SPRITE_ARB);
		}
	}

/*
	if ( mBlending ) {
		aeDisable(GL_BLEND);
	}
*/

	aeUnbind(mColorMap);
	aeUnbind(mMaterial);

	if ( depthMask ) {
		aeDepthMask(GL_TRUE);
	}
	if ( lighting ) {
		aeEnable(GL_LIGHTING);
	}

	aePopAttrib();
}

void Particle::reset()
{
	Particle_s  *pParticle;
	Particle_s **ppParticle;

	ppParticle = &mActiveList;				// Start at the head of the active list

	while ( *ppParticle ) {
		pParticle = *ppParticle;			// Set a pointer to the head

		// Put the particle back on the free list...
		*ppParticle = pParticle->mNext;
		pParticle->mNext = mFreeList;
		mFreeList = pParticle;

		--mActiveCount;
	}
}

void Particle::setAirResistence(bool airResistence)
{
	mAirResistence = airResistence;
}

void Particle::setAlphaValue(float alpha)
{
	mAlpha = alpha;
}

void Particle::setAttenuation(const vector3f& attenuation)
{
	mAttenuation = attenuation;
}

void Particle::setBlending(bool state)
{
	mBlending = state;
}

void Particle::setBoundingVolume()
{
	vector3f size = mSize * mScale;

	aeDelete( mBoundingVolume );
	switch ( mVolumeType ) {
		case IVolume::Type::AABBox:
		case IVolume::Type::AABBoxNoTranslate:
			mBoundingVolume = new Mesh::Volume::AABBoxVolume(size);
			break;
		case IVolume::Type::Box:
			mBoundingVolume = new Mesh::Volume::BoxVolume(size);
			break;
		case IVolume::Type::Cylinder:
			mBoundingVolume = new Mesh::Volume::CylinderVolume(size);
			break;
		case IVolume::Type::Sphere:
			mBoundingVolume = new Mesh::Volume::SphereVolume(size);
			break;
		default:
			assert(!"no default volume set!");
			break;
	}
}

void Particle::setCollisionPlane(const vector3f& vPlaneNormal, const vector3f& vPoint, 
								 float fBounceFactor, int nCollisionResult)
{
    Plane_s *pPlane = new Plane_s;

    pPlane->mNormal          = vPlaneNormal;
    pPlane->mPoint           = vPoint;
    pPlane->mBounceFactor    = fBounceFactor;
    pPlane->mCollisionResult = nCollisionResult;

    pPlane->mNext = mPlanes;	// Attach the curent list to it...
    mPlanes = pPlane;			// ... and make it the new head.
}

void Particle::setColor(const vector3f& color)
{
	mColor = color;
}

void Particle::setFadeInTime(float time)
{
	mFadeInTime = time;
}

void Particle::setFadeOutTime(float time)
{
	mFadeOutTime = time;
}

void Particle::setGlobalGravity(bool state)
{
	mApplyGlobalGravity = state;
}

void Particle::setGravity(const vector3f& gravity)
{
	mGravity = gravity;
}

void Particle::setLifeTime(float time)
{
	mLifeTime = time;
}

void Particle::setLoopExecution(bool state)
{
	mLoopExecution = state;
}

void Particle::setMaxParticles(int max)
{
	mMaxParticles = max;
}

void Particle::setMesh(Core::RenderObject *m)
{
	mMesh = m;
}

void Particle::setNumToRelease(int release)
{
	mNumToRelease = release;
}

void Particle::setParticleSize(float size)
{
	mParticleSize = size;
}

void Particle::setPolygonalMode(bool state)
{
	mPolygonalMode = state;
}

void Particle::setRange(const vector3f& range)
{
	mRange = range;
}

void Particle::setReleaseInterval(float interval)
{
	mReleaseInterval = interval;
}

void Particle::setVariableVelocity(const vector3f& variable)
{
	mVariableVelocity = variable;
}

void Particle::setVelocity(const vector3f& velocity)
{
	mVelocity = velocity;
}

void Particle::setVolumeType(IVolume::Type::E type)
{
	mVolumeType = type;
}

void Particle::unload()
{
	aeDeleteBuffer(mColorBuffer.mDriverId);
	aeDeleteBuffer(mVertexBuffer.mDriverId);

	while ( mPlanes ) {					// Repeat till null...
		Plane_s *pPlane = mPlanes;		// Hold onto the first one
		mPlanes = pPlane->mNext;		// Move down to the next one
		delete pPlane;					// Delete the one we're holding
	}
	mPlanes = 0;

	while ( mActiveList ) {
		Particle_s *pParticle = mActiveList;
		mActiveList = pParticle->mNext;
		delete pParticle;
	}
	mActiveList = 0;

	while ( mFreeList ) {
		Particle_s *pParticle = mFreeList;
		mFreeList = pParticle->mNext;
		delete pParticle;
	}
	mFreeList = 0;

	aeDelete( mBoundingVolume );
}

void Particle::update(float elpasedTime)
{
    Particle_s	*pParticle = 0;
    Particle_s	**ppParticle;
    Plane_s		*pPlane = 0;
    Plane_s		**ppPlane;
    vector3f	vOldPosition;

    mCurrentTime += elpasedTime;												// Update our particle system timer...

    ppParticle = &mActiveList;													// Start at the head of the active list

    while ( *ppParticle ) {
        pParticle = *ppParticle;												// Set a pointer to the head

        float fTimePassed  = mCurrentTime - pParticle->mInitTime;

		if ( fTimePassed >= mLifeTime ) {										// Time is up, put the particle back on the free list...
            *ppParticle = pParticle->mNext;
            pParticle->mNext = mFreeList;
            mFreeList = pParticle;

			if ( mLoopExecution ) {												// Only decrement mActiveCount if looping is set to true
				mActiveCount -= 1;
			}
        }
		else {																	// Update particle position, velocity and alpha value
            pParticle->mCurVel += mGravity * elpasedTime;						// Update velocity with respect to Gravity (Constant Accelaration)
			if ( mApplyGlobalGravity ) {
			//	pParticle->mCurVel += Map.vGravity * elpasedTime;
			}

			if ( mAirResistence ) {												// Update velocity with respect to Wind (Accelaration based on difference of vectors)
			//	pParticle->mCurVel += (Map.vWind - pParticle->mCurVel) * elpasedTime;
			}

			// Update alpha value of particle according to life time
			if ( fTimePassed < mFadeInTime ) {
				pParticle->mAlpha = fTimePassed / mFadeInTime;
			}
			else if ( mLifeTime - fTimePassed < mFadeOutTime) {
				pParticle->mAlpha = (mLifeTime - fTimePassed) / mFadeOutTime;
			}
			else {
				pParticle->mAlpha = 1.0f;
			}

            // Finally, update position with respect to velocity
            vOldPosition = pParticle->mCurPos;
            pParticle->mCurPos += pParticle->mCurVel * elpasedTime;

            //-----------------------------------------------------------------
            // BEGIN Checking the particle against each plane that was set up

            ppPlane = &mPlanes; // Set a pointer to the head

            while ( *ppPlane ) {
                pPlane = *ppPlane;
                int result = classifyPoint( &pParticle->mCurPos, pPlane );

                if ( result == CP_BACK /*|| result == CP_ONPLANE */ ) {
                    if ( pPlane->mCollisionResult == CR_BOUNCE ) {
                        pParticle->mCurPos = vOldPosition;

						//-----------------------------------------------------------------
						//
						// The new velocity vector of a particle that is bouncing off
						// a plane is computed as follows:
						//
						// Vn = (N.V) * N
						// Vt = V - Vn
						// Vp = Vt - Kr * Vn
						//
						// Where:
						// 
						// .  = Dot product operation
						// N  = The normal of the plane from which we bounced
						// V  = Velocity vector prior to bounce
						// Vn = Normal force
						// Kr = The coefficient of restitution ( Ex. 1 = Full Bounce, 
						//      0 = Particle Sticks )
						// Vp = New velocity vector after bounce
						//
						//-----------------------------------------------------------------

                        float Kr = pPlane->mBounceFactor;

                        vector3f Vn = vector3f::dotProduct( pPlane->mNormal, pParticle->mCurVel ) * pPlane->mNormal;
                        vector3f Vt = pParticle->mCurVel - Vn;
                        vector3f Vp = Vt - Kr * Vn;

                        pParticle->mCurVel = Vp;
                    }
                    else if ( pPlane->mCollisionResult == CR_RECYCLE ) {
                        pParticle->mInitTime -= mLifeTime;
                    }
                    else if ( pPlane->mCollisionResult == CR_STICK ) {
                        pParticle->mCurPos = vOldPosition;
                        pParticle->mCurVel = vector3f(0.0f,0.0f,0.0f);
                    }
                }

                ppPlane = &pPlane->mNext;
            }

            // END Plane Checking
            //-----------------------------------------------------------------

            ppParticle = &pParticle->mNext;
        }
    }

    //-------------------------------------------------------------------------
    // Emit new particles in accordance to the flow rate...
    // 
    // NOTE: The system operates with a finite number of particles.
    //       New particles will be created until the max amount has
    //       been reached, after that, only old particles that have
    //       been recycled can be reintialized and used again.
    //-------------------------------------------------------------------------

    if ( mCurrentTime - mLastUpdate > mReleaseInterval ) {
        // Reset update timing...
        mLastUpdate = mCurrentTime;

        // Emit new particles at specified flow rate...
		for ( int i = 0; i < mNumToRelease; i += 1 ) {
            // Do we have any free particles to put back to work?
            if ( mFreeList ) {
                // If so, hand over the first free one to be reused.
                pParticle = mFreeList;
                // Then make the next free particle in the list next to go!
                mFreeList = pParticle->mNext;
            }
            else {
                // There are no free particles to recycle...
                // We'll have to create a new one from scratch!
                if ( mActiveCount < mMaxParticles ) {
					if ( NULL == ( pParticle = new Particle_s ) ) {
                        return;// E_OUTOFMEMORY;
					}
                }
            }

            if ( !mPause && mActiveCount < mMaxParticles ) {
                pParticle->mNext = mActiveList;									// Make it the new head...
                mActiveList = pParticle;

                // Set the attributes for our new particle...
				pParticle->mAlpha = 0.f;
                pParticle->mCurVel = mVelocity;

                if ( mVariableVelocity != vector3f(0.f, 0.f, 0.f) ) {
                    vector3f vRandomVec = getRandomVector();
                    pParticle->mCurVel += vRandomVec * mVariableVelocity;
                }

                pParticle->mInitTime = mCurrentTime;
				vector3f pos = getTranslation();
                pParticle->mCurPos = pos;

				pParticle->mCurPos.x = pos.x + (mRange.x > 1.f ? (float)(RangedRandom(0, (int)mRange.x)) : 0.f);
				pParticle->mCurPos.y = pos.y + (mRange.y > 1.f ? (float)(RangedRandom(0, (int)mRange.y)) : 0.f);
				pParticle->mCurPos.z = pos.z + (mRange.z > 1.f ? (float)(RangedRandom(0, (int)mRange.z)) : 0.f);

				mActiveCount += 1;
            }
        }
    }
}


}
