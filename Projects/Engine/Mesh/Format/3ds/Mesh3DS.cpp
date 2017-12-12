
// Header
#include "Mesh3DS.h"

// Library includes

// Project includes
#include <Math/Normals.h>
#include <Mesh/Volume/Volumes.h>
#include <Mesh/Utils.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


Mesh3DS::Mesh3DS(Core::AEid id, const std::string& name)
: AMesh(id, name)
{
}

Mesh3DS::~Mesh3DS()
{
	unload();
}

void Mesh3DS::generateIndexBuffer()
{
	unsigned int polygon_count = mMesh.polygon.size();

	mMesh.index.resize(polygon_count * 3);

	unsigned int index = 0;		// Index in the index buffer
	for ( unsigned int i = 0; i < mMesh.polygon.size(); ++i ) {
		mMesh.index[index++] = mMesh.polygon[i].x;
		mMesh.index[index++] = mMesh.polygon[i].y;
		mMesh.index[index++] = mMesh.polygon[i].z;
	}
}

void Mesh3DS::generateNormals()
{
	mMesh.normal.resize(mMesh.vertex.size());

    for ( size_t i = 0; i < mMesh.index.size(); i += 3 ) {
		vector3f v0 = mMesh.vertex[ mMesh.index[i + 0] ];
        vector3f v1 = mMesh.vertex[ mMesh.index[i + 1] ];
        vector3f v2 = mMesh.vertex[ mMesh.index[i + 2] ];

		vector3f normal = normalize( cross( v1 - v0, v2 - v0 ) );

		mMesh.normal[ mMesh.index[i + 0] ] += normal;
        mMesh.normal[ mMesh.index[i + 1] ] += normal;
        mMesh.normal[ mMesh.index[i + 2] ] += normal;
    }

    //const vector3f UP( 0.0f, 1.0f, 0.0f );
    for ( size_t i = 0; i < mMesh.normal.size(); ++i ) {
        mMesh.normal[i] = normalize( mMesh.normal[i] );
    }
}

void Mesh3DS::generateVertexBuffers()
{
	aeCreateBuffer(mMesh.mGLColorBuffer);
	aeCreateBuffer(mMesh.mGLIndexBuffer);
	aeCreateBuffer(mMesh.mGLNormalBuffer);
	aeCreateBuffer(mMesh.mGLTex0Buffer);
	aeCreateBuffer(mMesh.mGLVertexBuffer);

	// Copy the host data into the vertex buffer objects
	glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLVertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector3f) * mMesh.vertex.size(), &(mMesh.vertex[0]), GL_STATIC_DRAW );

	if ( !mMesh.color.empty() ) {
		glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLColorBuffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vector4<float>) * mMesh.color.size(), &(mMesh.color[0]), GL_STATIC_DRAW );
	}
	if ( !mMesh.normal.empty() ) {
		glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLNormalBuffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vector3f) * mMesh.normal.size(), &(mMesh.normal[0]), GL_STATIC_DRAW );
	}
	if ( !mMesh.texture.empty() ) {
		glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLTex0Buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vector2<float>) * mMesh.texture.size(), &(mMesh.texture[0]), GL_STATIC_DRAW );
	}
	if ( !mMesh.index.empty() ) {
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mMesh.mGLIndexBuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(AEuint) * mMesh.index.size(), &(mMesh.index[0]), GL_STATIC_DRAW );
	}
}

void Mesh3DS::init()
{
	animated(false);

	preprocess();

	setBoundingVolume();
}

void Mesh3DS::preprocess()
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

void Mesh3DS::render()
{
	aePushMatrix();
	aePushAttrib(GL_ALL_ATTRIB_BITS);

	applyFaceCulling(mFacing);

	aeTranslate(getTranslation());
	aeRotate(getRotation());

	aeColor(mColor);

	if ( mMaterial )
		mMaterial->bind();

	if ( mShader ) {
		mShader->begin();

		if ( mNormalMap ) {
			aeActiveTexture(GL_TEXTURE1);
			glClientActiveTexture(GL_TEXTURE1);
			aeEnable(GL_TEXTURE_2D, true);

			mNormalMap->bind();
			mShader->set("normalMap", 1);

			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLTex0Buffer );
			glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );
		}
		if ( mHeightMap ) {
			aeActiveTexture(GL_TEXTURE2);
			glClientActiveTexture(GL_TEXTURE2);
			aeEnable(GL_TEXTURE_2D, true);

			mHeightMap->bind();
			mShader->set("heightMap", 2);

			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glBindBuffer( GL_ARRAY_BUFFER, mMesh.mGLTex0Buffer );
			glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );

			mShader->set("bias", -0.025f);
			mShader->set("enableParallax", 1);
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

		if ( mColorMap )
			mColorMap->bind();
		if ( mShader )
			mShader->set("colorMap", 0);

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
			glDrawElements( GL_TRIANGLES, mMesh.index.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

		glDisableClientState( GL_NORMAL_ARRAY );
		//glDisableClientState( GL_COLOR_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );

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

		aeActiveTexture( GL_TEXTURE0 );
		glClientActiveTexture(GL_TEXTURE0);

	aePopMatrix();

	glClientActiveTexture( GL_TEXTURE0 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glMatrixMode( GL_MODELVIEW );

	if ( mMaterial )
		mMaterial->unbind();
	if ( mColorMap )
		mColorMap->unbind();

	aePopAttrib();
	aePopMatrix();
}

void Mesh3DS::setAnimation(const std::string& /*animation*/)
{
	// 3ds meshes are just static objects so far
}

void Mesh3DS::setBoundingVolume()
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

void Mesh3DS::setMesh(const mesh3ds_vbo& mesh)
{
	mMesh = mesh;
}

void Mesh3DS::unload()
{
	aeDeleteBuffer(mMesh.mGLColorBuffer);
	aeDeleteBuffer(mMesh.mGLIndexBuffer);
	aeDeleteBuffer(mMesh.mGLNormalBuffer);
	aeDeleteBuffer(mMesh.mGLTex0Buffer);
	aeDeleteBuffer(mMesh.mGLVertexBuffer);

	aeDelete(mBoundingVolume);
}


}
