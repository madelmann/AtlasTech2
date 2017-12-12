
// Header
#include "MeshOBJ.h"

// Library includes

// Project includes
#include <Math/Normals.h>
#include <Mesh/Volume/Volumes.h>
#include <Mesh/Utils.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


MeshOBJ::MeshOBJ(Core::AEid id, const std::string& name)
: AMesh(id, name)
{
}

MeshOBJ::~MeshOBJ()
{
	unload();
}

void MeshOBJ::generateIndexBuffer()
{
}

void MeshOBJ::generateNormals()
{
}

void MeshOBJ::generateVertexBuffers()
{
}

void MeshOBJ::init()
{
	animated(false);

	preprocess();

	setBoundingVolume();
}

void MeshOBJ::preprocess()
{
	// scale mesh
	if ( mScale != vector3f(1.f, 1.f, 1.f) ) {
		for ( VertexBuffer::iterator it = mMesh.vertex.begin(); it != mMesh.vertex.end(); ++it ) {
			(*it) = (*it) * mScale;
		}
	}

	generateIndexBuffer();
	generateNormals();
	generateVertexBuffers();
}

void MeshOBJ::render()
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

			if ( mNormalMap ) {
				aeActiveTexture(GL_TEXTURE1);
				glClientActiveTexture(GL_TEXTURE1);
				aeEnable(GL_TEXTURE_2D, true);

				mShader->set("normalMap", 1);
				mNormalMap->bind();

				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLTex0Buffer );
				glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );
			}
			if ( mHeightMap ) {
				aeActiveTexture(GL_TEXTURE2);
				glClientActiveTexture(GL_TEXTURE2);
				aeEnable(GL_TEXTURE_2D, true);

				mShader->set("heightMap", 2);
				mHeightMap->bind();

				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLTex0Buffer );
				glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );

				mShader->set("bias", -0.025f);
				mShader->set("enableParallax", 0);
				mShader->set("scale", 0.04f);
			}
		}


		aeActiveTexture( GL_TEXTURE0 );
		glClientActiveTexture(GL_TEXTURE0);
		aeEnable(GL_TEXTURE_2D);

		glMatrixMode( GL_TEXTURE );
		aePushMatrix();
			aeActiveTexture( GL_TEXTURE0 );
			glClientActiveTexture(GL_TEXTURE0);

			if ( mShader && mColorMap ) {
				mShader->set("colorMap", 0);
				mColorMap->bind();
			}

			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLTex0Buffer );
			glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );

			glEnableClientState( GL_VERTEX_ARRAY );
			//glEnableClientState( GL_COLOR_ARRAY );
			glEnableClientState( GL_NORMAL_ARRAY );

				glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLVertexBuffer );
				glVertexPointer( 3, GL_FLOAT, 0, BUFFER_OFFSET(0) );
				//glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLColorBuffer );
				//glColorPointer( 4, GL_FLOAT, 0, BUFFER_OFFSET(0) );
				glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLNormalBuffer );
				glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(0) );

				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mMesh.mGLIndexBuffer );
				glDrawElements( GL_TRIANGLES, mMesh.vertex_index.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

			glDisableClientState( GL_NORMAL_ARRAY );
			//glDisableClientState( GL_COLOR_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );

			if ( mShader ) {
				if ( mNormalMap ) {
					if ( mHeightMap ) {
						aeActiveTexture(GL_TEXTURE2);
						aeDisable(GL_TEXTURE_2D, true);
					}

					aeActiveTexture(GL_TEXTURE1);
					aeDisable(GL_TEXTURE_2D, true);
				}

				mShader->end();
			}

			aeActiveTexture( GL_TEXTURE0 );
			glClientActiveTexture(GL_TEXTURE0);

		aePopMatrix();

		glClientActiveTexture( GL_TEXTURE0 );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

		glMatrixMode( GL_MODELVIEW );

		if ( mMaterial ) {
			mMaterial->unbind();
		}

	aePopAttrib();
	//aePopAttrib();
	aePopAttrib();
}

void MeshOBJ::setAnimation(const std::string& /*animation*/)
{
	// obj meshes are just static objects so far
}

void MeshOBJ::setBoundingVolume()
{
	float coordXL = 0.0f;
	float coordXR = 0.0f;
	float coordYL = 0.0f;
	float coordYR = 0.0f;
	float coordZL = 0.0f;
	float coordZR = 0.0f;

	coordXL = mMesh.vertex[0].x;
	coordXR = mMesh.vertex[0].x;
	coordYL = mMesh.vertex[0].y;
	coordYR = mMesh.vertex[0].y;
	coordZL = mMesh.vertex[0].z;
	coordZR = mMesh.vertex[0].z;

	for ( unsigned int i = 1; i < mMesh.vertex.size(); ++i ) {
		if ( mMesh.vertex[i].x < coordXL ) {
			coordXL = mMesh.vertex[i].x;
		}
		if ( mMesh.vertex[i].x > coordXR ) {
			coordXR = mMesh.vertex[i].x;
		}
		if ( mMesh.vertex[i].y < coordYL ) {
			coordYL = mMesh.vertex[i].y;
		}
		if ( mMesh.vertex[i].y > coordYR ) {
			coordYR = mMesh.vertex[i].y;
		}
		if ( mMesh.vertex[i].z < coordZL ) {
			coordZL = mMesh.vertex[i].z;
		}
		if ( mMesh.vertex[i].z > coordZR ) {
			coordZR = mMesh.vertex[i].z;
		}
	}

	mSize.x = coordXR - coordXL;
	mSize.y = coordYR - coordYL;
	mSize.z = coordZR - coordZL;

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

void MeshOBJ::setMesh(obj_t mesh)
{
	mMesh = mesh;
}

void MeshOBJ::unload()
{
	aeDeleteBuffer(mMesh.mGLColorBuffer);
	aeDeleteBuffer(mMesh.mGLIndexBuffer);
	aeDeleteBuffer(mMesh.mGLNormalBuffer);
	aeDeleteBuffer(mMesh.mGLTex0Buffer);
	aeDeleteBuffer(mMesh.mGLVertexBuffer);

	aeDelete(mBoundingVolume);
}


}
