
// Header
#include "Water.h"

// Library includes

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Camera/IViewer.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Material/IManager.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Shader/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Light/Light.h>
#include <Material/Material.h>
#include <Mesh/Volume/AABBox.h>
#include <Mesh/Volume/Sphere.h>
#include <Mesh/Utils.h>
#include <Parsers/LineParser.h>
#include <Texture/Texture2D.h>
#include <Tools/Strings.h>
#include <Water/Plugins/ConsolePlugin.h>
#include "Settings.h"

// Namespace declarations


namespace WaterSpace {


Water::Water()
: mDepthMap(0),
  mDuDvMap(0),
  mLight(0),
  mMaterialManager(0),
  mMediaPathManager(0),
  mReflectionMap(0),
  mRefractionMap(0),
  mSettings(0),
  mShaderManager(0),
  mTexCoords(0.f, 1.f),
  mTextureManager(0),
  mTime(0.f),
  mViewer(0),
  mVisible(true)
{
}

Water::~Water()
{
	unload();
}

void Water::connectLight(Light *l)
{
	mLight = l;
}

void Water::connectMaterialManager(MaterialSpace::IManager *m)
{
	mMaterialManager = m;
}

void Water::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void Water::connectSettings(Settings *s)
{
	mSettings = s;
}

void Water::connectShaderManager(ShaderSpace::IManager *m)
{
	mShaderManager = m;
}

void Water::connectTextureManager(TextureSpace::IManager *m)
{
	mTextureManager = m;
}

void Water::connectViewer(IViewer *v)
{
	mViewer = v;
}

Texture2D* Water::getReflectionMap()
{
	return mReflectionMap;
}

void Water::init()
{
/*
	assert(mLight);
	assert(mMaterialManager);
	assert(mMediaPathManager);
	assert(mShaderManager);
	assert(mTextureManager);
	assert(mViewer);
*/

	assert(mSettings);

	aeDelete( mBoundingVolume );
	mBoundingVolume = new Mesh::Volume::AABBoxVolume(mSettings->size());

	update(0.f);
}

void Water::processWave()
{
	float interval = mSettings->waveInterval();
	vector3f pos = mSettings->position();

	// actual position = original position * sin(time * time between waves)
	mWavePosition.x = pos.x;
	mWavePosition.y = pos.y + mSettings->waveHeight() * sin(mTime * interval);
	mWavePosition.z = pos.z;
}

void Water::render()
{
	// Only render water if enabled
	if ( !mSettings->enabled() ) {
		return;
	}

	aeTranslate(mWavePosition);

	Mesh::applyFaceCulling(mFacing);

	// Turn on the first texture unit and bind the REFLECTION texture
	aeActiveTexture(GL_TEXTURE0);
	aeEnable(GL_TEXTURE_2D);
	if ( mReflectionMap ) {
		mReflectionMap->bind();
	}
	else {
		Texture2D::unbind();
	}

	// Turn on the second texture unit and bind the REFRACTION texture
	aeActiveTexture(GL_TEXTURE1);
	aeEnable(GL_TEXTURE_2D);
	if ( mRefractionMap ) {
		mRefractionMap->bind();
	}
	else {
		Texture2D::unbind();
	}

	// Turn on the third texture unit and bind the NORMAL MAP texture
	aeActiveTexture(GL_TEXTURE2);
	aeEnable(GL_TEXTURE_2D);
	if ( mNormalMap ) {
		mNormalMap->bind();
	}
	else {
		Texture2D::unbind();
	}

	// Turn on the fourth texture unit and bind the DUDV MAP texture
	aeActiveTexture(GL_TEXTURE3);
	aeEnable(GL_TEXTURE_2D);
	if ( mDuDvMap ) {
		mDuDvMap->bind();
	}
	else {
		Texture2D::unbind();
	}

	// Turn on the fifth texture unit and bind the DEPTH texture
	aeActiveTexture(GL_TEXTURE4);
	aeEnable(GL_TEXTURE_2D);
	if ( mDepthMap ) {
		mDepthMap->bind();
	}
	else {
		Texture2D::unbind();
	}

	float waterUV = 10.f;
	// Set the refraction map's UV coordinates to our global g_WaterUV
	//float refrUV = waterUV;
	// Set our normal map's UV scale and shrink it by kNormalMapScale
	float normalUV = waterUV * 0.25f;

	vector3f size = mSettings->size();
	size /= 2;

	if ( mShader ) {
		mShader->begin();

		GLhandleARB program = mShader->program();
		GLint uniform;

		if ( mReflectionMap ) {
			// Set the variable "reflection" to correspond to the first texture unit
			mShader->set("reflection", (int)mReflectionMap->mDriverId);
		}
		if ( mRefractionMap ) {
			// Set the variable "refraction" to correspond to the second texture unit
			mShader->set("refraction", (int)mRefractionMap->mDriverId);
		}
		if ( mNormalMap ) {
			// Set the variable "normalMap" to correspond to the third texture unit
			mShader->set("normalMap", (int)mNormalMap->mDriverId);
		}
		if ( mDuDvMap ) {
			// Set the variable "dudvMap" to correspond to the fourth texture unit
			mShader->set("dudvMap", (int)mDuDvMap->mDriverId);
		}
		if ( mDepthMap ) {
			// Set the variable "depthMap" to correspond to the fifth texture unit
			mShader->set("depthMap", (int)mDepthMap->mDriverId);
		}

		// Give the variable "waterColor" a blue color
		uniform = glGetUniformLocationARB(program, "waterColor");
		glUniform4fARB(uniform, mColor.x, mColor.y, mColor.z, 1.0f);

		// We don't use lighting in this tutorial, but we do need to calculate
		// the diffuse and specular lighting on the water to increase realism.
		vector3f light = mLight->position();

		// Give the variable "lightPos" our light position
		uniform = glGetUniformLocationARB(program, "lightPos");
		glUniform4fARB(uniform, light.x, -light.y, light.z, 1.0f); 

		// Store the camera position in a variable
		vector3f camera = mViewer->position();

		// Give the variable "cameraPos" our camera position
		uniform = glGetUniformLocationARB(program, "cameraPos");
		glUniform4fARB(uniform, camera.x, camera.y, camera.z, 1.0f);

		mShader->set("fresnelMode", 0);
	}

	// Draw our huge water quad
	aeBegin(GL_QUADS);
		// dudv & depth map coordinates will be set in shader,
		// so do not set them yet!

		// The back left vertice for the water
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, mTexCoords.x, normalUV + mTexCoords.y);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, mTexCoords.x, normalUV + mTexCoords.y);
		//glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 0.0f);
		//glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 0.0f);
		aeVertex3f(mWavePosition.x - size.x, mWavePosition.y, mWavePosition.z - size.z);

		// The front left vertice for the water
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, waterUV);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, mTexCoords.x, mTexCoords.y);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, mTexCoords.x, mTexCoords.y);
		//glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 0.0f);
		//glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 0.0f);
		aeVertex3f(mWavePosition.x - size.x, mWavePosition.y, mWavePosition.z + size.z);

		// The front right vertice for the water
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, waterUV, waterUV);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, normalUV + mTexCoords.x, mTexCoords.y);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, normalUV + mTexCoords.x, mTexCoords.y);
		//glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 0.0f);
		//glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 0.0f);
		aeVertex3f(mWavePosition.x + size.x, mWavePosition.y, mWavePosition.z + size.z);

		// The back right vertice for the water
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, waterUV, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, normalUV + mTexCoords.x, normalUV + mTexCoords.y);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, normalUV + mTexCoords.x, normalUV + mTexCoords.y);
		//glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f, 0.0f);
		//glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0.0f, 0.0f);
		aeVertex3f(mWavePosition.x + size.x, mWavePosition.y, mWavePosition.z - size.z);
	aeEnd();

	if ( mShader ) {
		mShader->end();
	}


	// Turn the fifth multi-texture pass off
	aeActiveTexture(GL_TEXTURE4);
	aeDisable(GL_TEXTURE_2D);

	// Turn the fourth multi-texture pass off
	aeActiveTexture(GL_TEXTURE3);
	aeDisable(GL_TEXTURE_2D);

	// Turn the third multi-texture pass off
	aeActiveTexture(GL_TEXTURE2);
	aeDisable(GL_TEXTURE_2D);

	// Turn the second multi-texture pass off
	aeActiveTexture(GL_TEXTURE1);
	aeDisable(GL_TEXTURE_2D);

	// Reset multi texture to first texture slot
	aeActiveTexture(GL_TEXTURE0);
}

void Water::setDepthMap(Texture2D *map)
{
	mDepthMap = map;
}

void Water::setDuDvMap(Texture2D *map)
{
	mDuDvMap = map;
}

void Water::setReflectionMap(Texture2D *map)
{
	mReflectionMap = map;
}

void Water::setRefractionMap(Texture2D *map)
{
	mRefractionMap = map;
}

void Water::unload()
{
	delete mBoundingVolume;
	mBoundingVolume = 0;
}

void Water::update(float elapsedTime)
{
	if ( !mSettings->enabled() ) {
		return;
	}

	mTime += elapsedTime;

	mTexCoords.x += mSettings->flowSpeed().x * elapsedTime;
	mTexCoords.y += mSettings->flowSpeed().y * elapsedTime;

	if ( mTexCoords.x > 1.f ) {
		mTexCoords.x = 0.f;
	}
	if ( mTexCoords.y > 2.f ) {
		mTexCoords.y = 1.f;
	}

	processWave();
}


}
