
// Header
#include "Manager.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Interfaces/Texture/IManager.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Misc/Colors.h>
#include <Texture/Texture.h>

// Namespace declarations


namespace ApplicationSpace {
namespace Terrain {


Manager::Manager()
: mColorMap(0),
  mTextureManager(0)
{
}

Manager::~Manager()
{
}

void Manager::clear()
{
}

void Manager::connectTextureManager(TextureSpace::IManager *m)
{
	mTextureManager = m;
}

void Manager::configurationComplete()
{
	assert(!mConfigurationComplete);

	assert(mTextureManager);

	mConfigurationComplete = true;
}

vector3f Manager::heightAt(const vector3f& pos)
{
	return pos;
}

void Manager::init()
{
	mColorMap = mTextureManager->create("invalid");

	mIsReady = true;
}

void Manager::manage()
{
}

vector3f Manager::normalAt(const vector3f& pos)
{
(void)pos;
	return vector3f(1.f, 1.f, 1.f);
}

void Manager::render()
{
	aePushMatrix();

	aeColor(Color::blue);

	//aeColor(mColor);

	//if ( mMaterial ) {
	//	mMaterial->bind();
	//}

	//if ( mShader ) {
	//	mShader->begin();

	//	if ( mNormalMap ) {
	//		aeActiveTexture(GL_TEXTURE1);
	//		glClientActiveTexture(GL_TEXTURE1);
	//		aeForceEnable(GL_TEXTURE_2D);

	//		mShader->set("normalMap", 1);
	//		mNormalMap->bind();
	//	}
	//	if ( mHeightMap ) {
	//		aeActiveTexture(GL_TEXTURE2);
	//		glClientActiveTexture(GL_TEXTURE2);
	//		aeForceEnable(GL_TEXTURE_2D);

	//		mShader->set("heightMap", 2);
	//		mHeightMap->bind();

	//		mShader->set("bias", -0.025f);
	//		mShader->set("enableParallax", 0);
	//		mShader->set("scale", 0.04f);
	//	}
	//}

	//if ( mShader ) {
	//	mShader->set("colorMap", 0);
	//}

	aeActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);

	if ( mColorMap ) {
		mColorMap->bind();
	}

	vector3f mSize(2048.f, 0.f, 2048.f);
	vector2f mTextureCoords(0.f, 0.f);


	aeBegin(GL_QUADS);
		aeTexCoord2f(mTextureCoords.x, mTextureCoords.y);				aeVertex3f(0.f, 0.f, 0.f);
		aeTexCoord2f(mTextureCoords.x + 1.f, mTextureCoords.y);			aeVertex3f(0.f, 0.f, mSize.z);
		aeTexCoord2f(mTextureCoords.x + 1.f, mTextureCoords.y + 1.0f);	aeVertex3f(mSize.x, 0.f, mSize.z);
		aeTexCoord2f(mTextureCoords.x, mTextureCoords.y + 1.0f);		aeVertex3f(mSize.x, 0.f, 0.f);
	aeEnd();

	if ( mColorMap ) {
		mColorMap->unbind();
	}


	aeColor(Color::white);
	glLineWidth(3.f);

	vector2f sub = vector2f(16.f, 16.f);

	aeBegin(GL_LINES);
		for ( int x = 0; x <= sub.x; ++x ) {
			aeVertex3f(x * (mSize.x / sub.x),	0.f,	0.f		);
			aeVertex3f(x * (mSize.x / sub.x),	0.f,	mSize.z	);
		}
		for ( int z = 0; z <= sub.y; ++z ) {
			aeVertex3f(0.f,		0.f,	z * (mSize.z / sub.y)	);
			aeVertex3f(mSize.x,	0.f,	z * (mSize.z / sub.y)	);
		}
	aeEnd();

	//if ( mShader ) {
	//	if ( mNormalMap ) {
	//		if ( mHeightMap ) {
	//			aeActiveTexture(GL_TEXTURE2);
	//			aeForceDisable(GL_TEXTURE_2D);
	//		}

	//		aeActiveTexture(GL_TEXTURE1);
	//		aeForceDisable(GL_TEXTURE_2D);
	//	}

	//	mShader->end();
	//}
	//if ( mMaterial ) {
	//	mMaterial->unbind();
	//}

	aePopMatrix();
}

void Manager::shutdown()
{
}

void Manager::update(float /*elapsedTime*/)
{
}


}
}
