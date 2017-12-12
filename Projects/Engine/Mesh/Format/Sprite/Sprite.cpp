
// Header
#include "Sprite.h"

// Library includes

// Project includes
#include <Mesh/Volume/Volumes.h>
#include <Mesh/Utils.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


Sprite::Sprite()
: mLastAnimTime(0.f),
  mMeshId(0),
  mTextureAnimation(false)
{
}

Sprite::~Sprite()
{
	unload();
}

void Sprite::init()
{
	setBoundingVolume();

	// Destroy display list if already exists.
	if ( aeIsList(mMeshId) ) {
		aeDeleteLists(mMeshId, 1);
	}

	// Ask OpenGL for a new display list number
	mMeshId = aeGenLists(1);

	if ( mMeshId == 0 ) {
		//TODO : throw an exception
		return;
	}

	aeNewList(mMeshId, GL_COMPILE);

		renderWithoutDisplayList();

	aeEndList();
}

void Sprite::render()
{
	aeTranslate(getTranslation());
	aeRotate(getRotation());

	aePushAttrib(GL_CULL_FACE);
	aePushAttrib(GL_LIGHTING);

		applyFaceCulling(mFacing);

		//aeDepthMask(false);

		renderWithoutDisplayList();

		//aeDepthMask(true);

	aePopAttrib();
	aePopAttrib();
}

void Sprite::renderWithoutDisplayList()
{
	aeColor(mColor);

	if ( mMaterial ) {
		mMaterial->bind();
	}
	if ( mShader ) {
		mShader->begin();

		if ( mNormalMap ) {
			aeActiveTexture(GL_TEXTURE1);
			glClientActiveTexture(GL_TEXTURE1);
			aeForceEnable(GL_TEXTURE_2D);

			mShader->set("normalMap", 1);
			mNormalMap->bind();
		}
		if ( mHeightMap ) {
			aeActiveTexture(GL_TEXTURE2);
			glClientActiveTexture(GL_TEXTURE2);
			aeForceEnable(GL_TEXTURE_2D);

			mShader->set("heightMap", 2);
			mHeightMap->bind();

			mShader->set("bias", -0.025f);
			mShader->set("enableParallax", 0);
			mShader->set("scale", 0.04f);
		}
	}

	if ( mShader ) {
		mShader->set("colorMap", 0);
	}

	aeActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);

	if ( mColorMap ) {
		mColorMap->bind();
	}

	float hx = mSize.x / 2;
	float hz = mSize.z / 2;

	aeBegin(GL_QUADS);
		aeTexCoord2f(mTextureCoords.x, mTextureCoords.y);				aeVertex3f(-hx, 0.f, -hz);
		aeTexCoord2f(mTextureCoords.x + 1.f, mTextureCoords.y);			aeVertex3f(-hx, 0.f,  hz);
		aeTexCoord2f(mTextureCoords.x + 1.f, mTextureCoords.y + 1.0f);	aeVertex3f( hx, 0.f,  hz);
		aeTexCoord2f(mTextureCoords.x, mTextureCoords.y + 1.0f);		aeVertex3f( hx, 0.f, -hz);
	aeEnd();

	if ( mShader ) {
		if ( mNormalMap ) {
			if ( mHeightMap ) {
				aeActiveTexture(GL_TEXTURE2);
				aeForceDisable(GL_TEXTURE_2D);
			}

			aeActiveTexture(GL_TEXTURE1);
			aeForceDisable(GL_TEXTURE_2D);
		}

		mShader->end();
	}
	if ( mMaterial ) {
		mMaterial->unbind();
	}
}

void Sprite::setAnimation(const std::string& animation)
{
	if ( !hasAnimation(animation) ) {
		return;
	}


}

void Sprite::setTextureCoordAnim(const vector2f& anim)
{
	mTextureCoordAnim = anim;

	if ( mTextureCoordAnim.x || mTextureCoordAnim.y ) {
		mTextureAnimation = true;
	}
}

void Sprite::setTextureCoords(const vector2f& coords)
{
	mTextureCoords = coords;
}

void Sprite::unload()
{
	// Destroy display list if it exists.
	if ( aeIsList(mMeshId) ) {
		aeDeleteLists(mMeshId, 1);
	}

	aeDelete(mBoundingVolume);
}

void Sprite::update(float elapsedTime)
{
	/*
	// If our elapsed time goes over a 5th of a second, we start over and go to the next key frame
	if ( elapsedTime >= (1000.0f / mAnimationSpeed) ) {
		// Set our current frame to the next key frame (which could be the start of the anim)
		//mModel->currentFrame = (mModel->currentFrame + 1) % mModel->pAnimations[mModel->currentAnim].endFrame;

		// Set our last time to the current time just like we would when getting our FPS.
		mLastAnimTime = (float)GetTickCount();
	}
	*/

	if ( mTextureAnimation && mTextureCoordAnim.x ) {
		mTextureCoords.x += mTextureCoordAnim.x * elapsedTime;
		if ( mTextureCoords.x < 0.f ) {
			mTextureCoords.x = 1.f;
		}
		if ( mTextureCoords.x > 1.f ) {
			mTextureCoords.x = 0.f;
		}
	}
	if ( mTextureAnimation && mTextureCoordAnim.y ) {
		mTextureCoords.y += mTextureCoordAnim.y * elapsedTime;
		if ( mTextureCoords.y < 0.f ) {
			mTextureCoords.y = 1.f;
		}
		if ( mTextureCoords.y > 1.f ) {
			mTextureCoords.y = 0.f;
		}
	}
}


}
