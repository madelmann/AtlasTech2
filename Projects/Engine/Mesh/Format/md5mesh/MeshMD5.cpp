
// Header
#include "MeshMD5.h"

// Library includes

// Project includes
#include "MD5Model.h"
#include <Mesh/Volume/Volumes.h>
#include <Mesh/Utils.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


MeshMD5::MeshMD5()
: mLastAnimTime(0.f),
  mModel(0)
{
}

MeshMD5::~MeshMD5()
{
	unload();
}

void MeshMD5::init()
{
	setBoundingVolume();
}

void MeshMD5::render()
{
	aeTranslate(getTranslation());
	aeRotate(getRotation());

	aePushAttrib(GL_CULL_FACE);
	//aePushAttrib(GL_LIGHTING);
	aePushAttrib(GL_TEXTURE_2D);

		applyFaceCulling(mFacing);

		aeColor(mColor);

		if ( mMaterial ) {
			mMaterial->bind();
		}

		if ( mShader ) {
			mShader->begin();
		}

		if ( mColorMap ) {
			aeActiveTexture(GL_TEXTURE0);
			aeEnable(GL_TEXTURE_2D);

			if ( mShader ) {
				mShader->set("colorMap", 0);
			}

			mColorMap->bind();
		}

		if ( mShader && mNormalMap ) {
			aeActiveTexture(GL_TEXTURE1);
			aeEnable(GL_TEXTURE_2D);

			mShader->set("normalMap", 1);
			mNormalMap->bind();
		}

		if ( mShader && mHeightMap ) {
			aeActiveTexture(GL_TEXTURE2);
			aeEnable(GL_TEXTURE_2D);

			mShader->set("heightMap", 2);
			mHeightMap->bind();

			mShader->set("bias", -0.025f);
			mShader->set("enableParallax", 1);
			mShader->set("scale", 0.04f);
		}

		renderWithoutDisplayList();

		if ( mShader ) {
			aeActiveTexture(GL_TEXTURE2);
			aeDisable(GL_TEXTURE_2D);

			aeActiveTexture(GL_TEXTURE1);
			aeDisable(GL_TEXTURE_2D);

			aeActiveTexture(GL_TEXTURE0);
			aeEnable(GL_TEXTURE_2D);

			mShader->end();
		}

		if ( mMaterial ) {
			mMaterial->unbind();
		}

	aePopAttrib();
	//aePopAttrib();
	aePopAttrib();
}

void MeshMD5::renderWithoutDisplayList()
{
	mModel->render();
}

//float MeshMD5::returnCurrentTime(int nextFrame)
//{
//	static float elapsedTime = 0.0f;
//
//	// This function is very similar to finding the frames per second.
//	// Instead of checking when we reach a second, we check if we reach
//	// 1 second / our animation speed. (1000 ms / mAnimationSpeed).
//	// That's how we know when we need to switch to the next key frame.
//	// In the process, we get the t value for how we are at to going to the
//	// next animation key frame.
//
//	// Get the current time in milliseconds
//	float time = (float)GetTickCount();
//
//	// Find the time that has elapsed since the last time that was stored
//	elapsedTime = time - mLastAnimTime;
//
//	// To find the current t we divide the elapsed time by the ratio of 1 second / our anim speed.
//	// Since we aren't using 1 second as our t = 1, we need to divide the speed by 1000
//	// milliseconds to get our new ratio, which is a 5th of a second.
//	float t = elapsedTime / (1000.0f / mAnimationSpeed);
//
//	// If our elapsed time goes over a 5th of a second, we start over and go to the next key frame
//	if ( elapsedTime >= (1000.0f / mAnimationSpeed) ) {
//		// Set our current frame to the next key frame (which could be the start of the anim)
//		//mModel->currentFrame = nextFrame;
//
//		// Set our last time to the current time just like we would when getting our FPS.
//		mLastAnimTime = time;
//	}
//
//	// Return the time t so we can plug this into our interpolation.
//	return t;
//}

void MeshMD5::setAnimation(const std::string& animation)
{
	if ( !hasAnimation(animation) ) {
		return;
	}


}

void MeshMD5::setBoundingVolume()
{
	//vector3f size = mSize;

	//float coordXL = 0.0f;
	//float coordXR = 0.0f;
	//float coordYL = 0.0f;
	//float coordYR = 0.0f;
	//float coordZL = 0.0f;
	//float coordZR = 0.0f;

	//if ( (int)mModel->pAnimations.capacity() <= 0 ) {
	//	return;
	//}

	//int frame = mModel->pAnimations[mModel->currentAnim].startFrame;

	//coordXL = mModel->pObject[frame].pVerts[0].x;
	//coordXR = mModel->pObject[frame].pVerts[0].x;
	//coordYL = mModel->pObject[frame].pVerts[0].y;
	//coordYR = mModel->pObject[frame].pVerts[0].y;
	//coordZL = mModel->pObject[frame].pVerts[0].z;
	//coordZR = mModel->pObject[frame].pVerts[0].z;
	//
	//for ( int j = 1; j < mModel->pObject[frame].numOfVerts; j += 1 ) {
	//	if ( mModel->pObject[frame].pVerts[j].x < coordXL ) {
	//			coordXL = mModel->pObject[frame].pVerts[j].x;
	//	}
	//	if ( mModel->pObject[frame].pVerts[j].x > coordXR ) {
	//			coordXR = mModel->pObject[frame].pVerts[j].x;
	//	}
	//	if ( mModel->pObject[frame].pVerts[j].y < coordYL ) {
	//			coordYL = mModel->pObject[frame].pVerts[j].y;
	//	}
	//	if ( mModel->pObject[frame].pVerts[j].y > coordYR ) {
	//			coordYR = mModel->pObject[frame].pVerts[j].y;
	//	}
	//	if ( mModel->pObject[frame].pVerts[j].z < coordZL ) {
	//			coordZL = mModel->pObject[frame].pVerts[j].z;
	//	}
	//	if ( mModel->pObject[frame].pVerts[j].z > coordZR ) {
	//			coordZR = mModel->pObject[frame].pVerts[j].z;
	//	}
	//}

	//size.x = coordXR * mScale.x - coordXL * mScale.x;
	//size.y = coordYR * mScale.y - coordYL * mScale.y;
	//size.z = coordZR * mScale.z - coordZL * mScale.z;

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

void MeshMD5::setModel(MD5Model *model)
{
	mModel = model;
}

void MeshMD5::unload()
{
	aeDelete(mBoundingVolume);
	aeDelete(mModel);
}

void MeshMD5::update(float elapsedTime)
{
	// If our elapsed time goes over a 5th of a second, we start over and go to the next key frame
	if ( elapsedTime >= (1000.0f / mAnimationSpeed) ) {
		// Set our current frame to the next key frame (which could be the start of the anim)
		//mModel->currentFrame = (mModel->currentFrame + 1) % mModel->pAnimations[mModel->currentAnim].endFrame;

		// Set our last time to the current time just like we would when getting our FPS.
		mLastAnimTime = (float)GetTickCount();
	}
}


}
