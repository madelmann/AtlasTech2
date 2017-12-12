
// Header
#include "MeshMD2.h"

// Library includes

// Project includes
#include "Md2.h"
#include <Mesh/Volume/Volumes.h>
#include <Mesh/Utils.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


MeshMD2::MeshMD2()
: mLastAnimTime(0.f),
  mModel(0)
{
}

MeshMD2::~MeshMD2()
{
	unload();
}

void MeshMD2::init()
{
	setBoundingVolume();
}

void MeshMD2::render()
{
	aePushMatrix();

		aePushAttrib(GL_ALL_ATTRIB_BITS);

		aeTranslate(getTranslation());
		aeRotate(getRotation());

		applyFaceCulling(mFacing);

		aeColor(mColor);

		if ( mMaterial ) {
			mMaterial->bind();
		}
		if ( mShader ) {
			mShader->begin();

			if ( mColorMap ) {
				mShader->set("colorMap", 0);
				mColorMap->bind();
			}

			if ( mNormalMap ) {
				aeActiveTexture(GL_TEXTURE1);
				glClientActiveTexture(GL_TEXTURE1);
				aeEnable(GL_TEXTURE_2D, true);

				mShader->set("normalMap", 1);
				mNormalMap->bind();
			}
			if ( mHeightMap ) {
				aeActiveTexture(GL_TEXTURE2);
				glClientActiveTexture(GL_TEXTURE2);
				aeEnable(GL_TEXTURE_2D, true);

				mShader->set("heightMap", 2);
				mHeightMap->bind();

				mShader->set("bias", -0.025f);
				mShader->set("enableParallax", 1);
				mShader->set("scale", 0.04f);
			}
		}

		renderWithoutDisplayList();

		if ( mShader ) {
			if ( mHeightMap ) {
				aeActiveTexture(GL_TEXTURE2);
				aeDisable(GL_TEXTURE_2D, true);
			}
			if ( mNormalMap ) {
				aeActiveTexture(GL_TEXTURE1);
				aeDisable(GL_TEXTURE_2D, true);
			}

			aeActiveTexture(GL_TEXTURE0);

			mShader->end();
		}

		if ( mMaterial ) {
			mMaterial->unbind();
		}
		if ( mColorMap ) {
			mColorMap->unbind();
		}

		aePopAttrib();
	aePopMatrix();
}

void MeshMD2::renderWithoutDisplayList()
{
	// Make sure we have valid objects just in case. (size() is in the vector class)
	if ( mModel->pObject.size() <= 0 ) {
		return;
	}

	// Here we grab the current animation that we are on from our model's animation list
	tAnimationInfo pAnim = mModel->pAnimations[mModel->currentAnim];

	int nextFrame = (mModel->currentFrame + 1) % pAnim.endFrame;

	// If the next frame is zero, that means that we need to start the animation over.
	// To do this, we set nextFrame to the starting frame of this animation.
	if ( nextFrame == 0 ) {
		nextFrame = pAnim.startFrame;
	}

	// Get the current key frame we are on
	t3DObject pFrame = mModel->pObject[mModel->currentFrame];

	// Get the next key frame we are interpolating too
	t3DObject pNextFrame = mModel->pObject[nextFrame];

	// Get the first key frame so we have an address to the texture and face information
	t3DObject pFirstFrame = mModel->pObject[0];

	float t = returnCurrentTime(nextFrame);

	// Start rendering lines or triangles, depending on our current rendering mode (Lft Mouse Btn)
	aeBegin(GL_TRIANGLES);
		// Go through all of the faces (polygons) of the current frame and draw them
		for ( int j = 0; j < pFrame.numOfFaces; j += 1 ) {
			// Go through each corner of the triangle and draw it.
			for ( int whichVertex = 0; whichVertex < 3; whichVertex += 1 ) {
				// Get the index for each point of the face
				int vertIndex = pFirstFrame.pFaces[j].vertIndex[whichVertex];

				// Get the index for each texture coordinate for this face
				int texIndex = pFirstFrame.pFaces[j].coordIndex[whichVertex];

				// Make sure there was a UVW map applied to the object. Notice that
				// we use the first frame to check if we have texture coordinates because
				// none of the other frames hold this information, just the first by design.
				if ( pFirstFrame.pTexVerts ) {									// Pass in the texture coordinate for this vertex
					aeTexCoord2f(pFirstFrame.pTexVerts[ texIndex ].x, pFirstFrame.pTexVerts[ texIndex ].y);
				}

				// Store the current and next frame's vertex
				vector3f p1 = vector3f(pFrame.pVerts[ vertIndex ].x, pFrame.pVerts[ vertIndex ].y, pFrame.pVerts[ vertIndex ].z);
				vector3f p2 = vector3f(pNextFrame.pVerts[ vertIndex ].x, pNextFrame.pVerts[ vertIndex ].y, pNextFrame.pVerts[ vertIndex ].z);
				vector3f p3 = (p1 + t * (p2 - p1)) * mScale + mTranslation;

				aeVertex3f(p3.x, p3.y, p3.z);
			}
		}
	aeEnd();
}

float MeshMD2::returnCurrentTime(int nextFrame)
{
	float t = mTimeSinceLastAnimation / mAnimationSpeed;

	// If our elapsed time goes over a 5th of a second, we start over and go to the next key frame
	if ( mTimeSinceLastAnimation >= mAnimationSpeed ) {
		// Set our current frame to the next key frame (which could be the start of the anim)
		mModel->currentFrame = nextFrame;

		mTimeSinceLastAnimation -= mAnimationSpeed;
	}

	// Return the time t so we can plug this into our interpolation.
	return t;
}

void MeshMD2::setAnimation(const std::string& animation)
{
	if ( !hasAnimation(animation) ) {
		return;
	}


}

void MeshMD2::setBoundingVolume()
{
	float coordXL = 0.0f;
	float coordXR = 0.0f;
	float coordYL = 0.0f;
	float coordYR = 0.0f;
	float coordZL = 0.0f;
	float coordZR = 0.0f;

	if ( mModel->pAnimations.capacity() <= 0 ) {
		return;
	}

	int frame = mModel->pAnimations[mModel->currentAnim].startFrame;

	coordXL = mModel->pObject[frame].pVerts[0].x;
	coordXR = mModel->pObject[frame].pVerts[0].x;
	coordYL = mModel->pObject[frame].pVerts[0].y;
	coordYR = mModel->pObject[frame].pVerts[0].y;
	coordZL = mModel->pObject[frame].pVerts[0].z;
	coordZR = mModel->pObject[frame].pVerts[0].z;
	
	for ( int j = 1; j < mModel->pObject[frame].numOfVerts; j += 1 ) {
		if ( mModel->pObject[frame].pVerts[j].x < coordXL ) {
				coordXL = mModel->pObject[frame].pVerts[j].x;
		}
		if ( mModel->pObject[frame].pVerts[j].x > coordXR ) {
				coordXR = mModel->pObject[frame].pVerts[j].x;
		}
		if ( mModel->pObject[frame].pVerts[j].y < coordYL ) {
				coordYL = mModel->pObject[frame].pVerts[j].y;
		}
		if ( mModel->pObject[frame].pVerts[j].y > coordYR ) {
				coordYR = mModel->pObject[frame].pVerts[j].y;
		}
		if ( mModel->pObject[frame].pVerts[j].z < coordZL ) {
				coordZL = mModel->pObject[frame].pVerts[j].z;
		}
		if ( mModel->pObject[frame].pVerts[j].z > coordZR ) {
				coordZR = mModel->pObject[frame].pVerts[j].z;
		}
	}

	mSize.x = coordXR * mScale.x - coordXL * mScale.x;
	mSize.y = coordYR * mScale.y - coordYL * mScale.y;
	mSize.z = coordZR * mScale.z - coordZL * mScale.z;

	aeDelete(mBoundingVolume);
	switch ( mVolumeType ) {
		case IVolume::Type::AABBox:
		case IVolume::Type::AABBoxNoTranslate:
			mBoundingVolume = new Mesh::Volume::AABBoxVolume(mSize);
			break;
		case IVolume::Type::Box:
			mBoundingVolume = new Mesh::Volume::BoxVolume(mSize);
			break;
		case IVolume::Type::Cylinder:
			mBoundingVolume = new Mesh::Volume::CylinderVolume(mSize);
			break;
		case IVolume::Type::Sphere:
			mBoundingVolume = new Mesh::Volume::SphereVolume(mSize);
			break;
		default:
			assert(!"no default volume set!");
			break;
	}
}

void MeshMD2::setModel(t3DModel *model)
{
	mModel = model;
}

void MeshMD2::unload()
{
	aeDelete(mBoundingVolume);
	aeDelete(mModel);
}


}
