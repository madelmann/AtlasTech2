
// Header
#include "SkyBox.h"

// Library includes

// Project includes
#include "Settings.h"
#include <Core/Configuration/Configurator.h>
#include <Driver/GLWrapper.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Math/Vector3.h>
#include <Mesh/Volume/Volumes.h>
#include <Parsers/LineParser.h>
#include <Texture/Texture.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace SkySpace {


SkyBox::SkyBox(Core::AEid id)
: ASky(id),
  mBack(0),
  mBottom(0),
  mFront(0),
  mLeft(0),
  mRight(0),
  mTop(0)
{
	mVolumeType = IVolume::Type::AABBoxNoTranslate;
}

SkyBox::~SkyBox()
{
	unload();
}

void SkyBox::preprocess()
{
	// Ask OpenGL for a new display list number
	mMeshId = aeGenLists(1);
	if ( mMeshId == 0 ) {
		//TODO : throw an exception
		return;
	}

	aeNewList(mMeshId, GL_COMPILE);
		aeTranslate(mSettings->position());

		if ( mSettings->useFaceCulling() ) {
			aeEnable(GL_CULL_FACE, true);
			aeCullFace(GL_BACK);
		}

		if ( mMaterial ) {
			mMaterial->bind();
		}

		aeActiveTexture(GL_TEXTURE0);
		glClientActiveTexture(GL_TEXTURE0);
		aeEnable(GL_TEXTURE_2D);

		// Bind the BACK texture of the sky map to the BACK side of the cube
		mBack->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		aeBegin(GL_QUADS);
			aeTexCoord2f(1.0f, 0.0f); aeVertex3f(mSize.x,		0.f,	0.f);
			aeTexCoord2f(1.0f, 1.0f); aeVertex3f(mSize.x,	mSize.y,	0.f); 
			aeTexCoord2f(0.0f, 1.0f); aeVertex3f(	0.f,	mSize.y,	0.f);
			aeTexCoord2f(0.0f, 0.0f); aeVertex3f(	0.f,		0.f,	0.f);
		aeEnd();

		// Bind the FRONT texture of the sky map to the FRONT side of the box
		mFront->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		aeBegin(GL_QUADS);
			aeTexCoord2f(1.0f, 0.0f); aeVertex3f(	0.f,		0.f,	mSize.z);
			aeTexCoord2f(1.0f, 1.0f); aeVertex3f(	0.f,	mSize.y,	mSize.z);
			aeTexCoord2f(0.0f, 1.0f); aeVertex3f(mSize.x,	mSize.y,	mSize.z); 
			aeTexCoord2f(0.0f, 0.0f); aeVertex3f(mSize.x,		0.f,	mSize.z);
		aeEnd();

		// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
		mBottom->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		aeBegin(GL_QUADS);
			aeTexCoord2f(1.0f, 0.0f); aeVertex3f(	0.f,	0.f,		0.f);
			aeTexCoord2f(1.0f, 1.0f); aeVertex3f(	0.f,	0.f,	mSize.z);
			aeTexCoord2f(0.0f, 1.0f); aeVertex3f(mSize.x,	0.f,	mSize.z); 
			aeTexCoord2f(0.0f, 0.0f); aeVertex3f(mSize.x,	0.f,		0.f);
		aeEnd();

		// Bind the TOP texture of the sky map to the TOP side of the box
		mTop->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		aeBegin(GL_QUADS);
			aeTexCoord2f(0.0f, 1.0f); aeVertex3f(mSize.x,	mSize.y,		0.f);
			aeTexCoord2f(0.0f, 0.0f); aeVertex3f(mSize.x,	mSize.y,	mSize.z); 
			aeTexCoord2f(1.0f, 0.0f); aeVertex3f(	0.f,	mSize.y,	mSize.z);
			aeTexCoord2f(1.0f, 1.0f); aeVertex3f(	0.f,	mSize.y,		0.f);
		aeEnd();

		// Bind the LEFT texture of the sky map to the LEFT side of the box
		mLeft->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		aeBegin(GL_QUADS);
			aeTexCoord2f(1.0f, 1.0f); aeVertex3f(0.f,	mSize.y,		0.f);	
			aeTexCoord2f(0.0f, 1.0f); aeVertex3f(0.f,	mSize.y,	mSize.z); 
			aeTexCoord2f(0.0f, 0.0f); aeVertex3f(0.f,		0.f,	mSize.z);
			aeTexCoord2f(1.0f, 0.0f); aeVertex3f(0.f,		0.f,		0.f);		
		aeEnd();

		// Bind the RIGHT texture of the sky map to the RIGHT side of the box
		mRight->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		aeBegin(GL_QUADS);
			aeTexCoord2f(0.0f, 0.0f); aeVertex3f(mSize.x,		0.f,		0.f);
			aeTexCoord2f(1.0f, 0.0f); aeVertex3f(mSize.x,		0.f,	mSize.z);
			aeTexCoord2f(1.0f, 1.0f); aeVertex3f(mSize.x,	mSize.y,	mSize.z); 
			aeTexCoord2f(0.0f, 1.0f); aeVertex3f(mSize.x,	mSize.y,		0.f);
		aeEnd();

		if ( mMaterial ) {
			mMaterial->unbind();
		}
	aeEndList();
}

void SkyBox::render()
{
	aePushAttrib(GL_FOG);
	aePushAttrib(GL_LIGHTING);

	if ( !mSettings->affectedByFog() ) {
		aeDisable(GL_FOG);
	}
	if ( !mSettings->affectedByLight() ) {
		aeDisable(GL_LIGHTING);
	}

	aeCallList(mMeshId);

	aePopAttrib();
	aePopAttrib();
}

void SkyBox::unload()
{
	// Destroy display list if one exists.
	if ( aeIsList(mMeshId) ) {
		aeDeleteLists(mMeshId, 1);
	}
}

void SkyBox::update(float /*elapsedTime*/)
{
	// nothing to do here
}


}
