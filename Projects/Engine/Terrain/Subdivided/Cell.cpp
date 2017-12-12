
// Header
#include "Cell.h"

// Library includes

// Project includes
#include <Core/EngineSettings.h>
#include <Driver/GLWrapper.h>
#include <Mesh/Volume/Volumes.h>
#include <Tools/Tools.h>

// Namespace declarations


#define BUFFER_OFFSET(i) ((char*)NULL + (i))
#define ENABLE_SLOPE_BASED_BLEND 0


namespace Terrain {
namespace Subdivided {


Cell::Cell()
: mAdditionalTexture(0),
  mAlphaTexture(0),
  mBaseTexture(0),
  mEngineSettings(0),
  mGLColorBuffer(0),
  mGLIndexBuffer(0),
  mGLNormalBuffer(0),
  mGLTex0Buffer(0),
  mGLTex1Buffer(0),
  mGLTex2Buffer(0),
  mGLVertexBuffer(0)
{
}

Cell::~Cell()
{
	unload();
}

void Cell::connectEngineSettings(Core::EngineSettings *s)
{
	assert(s);
	assert(!mEngineSettings);

	mEngineSettings = s;
}

void Cell::generateIndexBuffer()
{
	const unsigned int terrainWidth = mCellSize.x;
	const unsigned int terrainHeight = mCellSize.y;

	// 2 triangles for every quad of the terrain mesh
	const unsigned int numTriangles = ( terrainWidth - 1 ) * ( terrainHeight - 1 ) * 2;

	// 3 indices for each triangle in the terrain mesh
	mMesh.mIndexBuffer.resize( numTriangles * 3 );

	unsigned int index = 0; // Index in the index buffer
	for ( unsigned int j = 0; j < (terrainHeight - 1); ++j ) {
		for ( unsigned int i = 0; i < (terrainWidth - 1); ++i ) {
			int vertexIndex = ( j * terrainWidth ) + i;
			// Top triangle (T0)
			mMesh.mIndexBuffer[index++] = vertexIndex;                           // V0
			mMesh.mIndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
			mMesh.mIndexBuffer[index++] = vertexIndex + 1;                       // V1
			// Bottom triangle (T1)
			mMesh.mIndexBuffer[index++] = vertexIndex;                           // V0
			mMesh.mIndexBuffer[index++] = vertexIndex + terrainWidth;            // V2
			mMesh.mIndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
		}
	}
}

void Cell::generateNormals()
{
    for ( size_t i = 0; i < mMesh.mIndexBuffer.size(); i += 3 ) {
        vector3f v0 = mMesh.mPositionBuffer[ mMesh.mIndexBuffer[i + 0] ];
        vector3f v1 = mMesh.mPositionBuffer[ mMesh.mIndexBuffer[i + 1] ];
        vector3f v2 = mMesh.mPositionBuffer[ mMesh.mIndexBuffer[i + 2] ];

		vector3f normal = normalize( cross( v1 - v0, v2 - v0 ) );

        mMesh.mNormalBuffer[ mMesh.mIndexBuffer[i + 0] ] += normal;
        mMesh.mNormalBuffer[ mMesh.mIndexBuffer[i + 1] ] += normal;
        mMesh.mNormalBuffer[ mMesh.mIndexBuffer[i + 2] ] += normal;
    }

    const vector3f UP( 0.0f, 1.0f, 0.0f );
    for ( size_t i = 0; i < mMesh.mNormalBuffer.size(); ++i ) {
        mMesh.mNormalBuffer[i] = normalize( mMesh.mNormalBuffer[i] );

#if ENABLE_SLOPE_BASED_BLEND
        float fTexture0Contribution = saturate( dot( mMesh.mNormalBuffer[i], UP ) - 0.1f );
        mMesh.mColorBuffer[i] = vector4<float>( fTexture0Contribution, fTexture0Contribution, fTexture0Contribution, mMesh.mColorBuffer[i].w );
#endif
    }
}

void Cell::generateVertexBuffers()
{
	aeCreateBuffer(mGLColorBuffer);
	aeCreateBuffer(mGLIndexBuffer);
	aeCreateBuffer(mGLNormalBuffer);
	aeCreateBuffer(mGLTex0Buffer);
	aeCreateBuffer(mGLTex1Buffer);
	aeCreateBuffer(mGLTex2Buffer);
	aeCreateBuffer(mGLVertexBuffer);

	// Copy the host data into the vertex buffer objects
	glBindBuffer( GL_ARRAY_BUFFER, mGLVertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector3f) * mMesh.mPositionBuffer.size(), &(mMesh.mPositionBuffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLColorBuffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector4<float>) * mMesh.mColorBuffer.size(), &(mMesh.mColorBuffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLNormalBuffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector3f) * mMesh.mNormalBuffer.size(), &(mMesh.mNormalBuffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLTex0Buffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector2<float>) * mMesh.mTex0Buffer.size(), &(mMesh.mTex0Buffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLTex1Buffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector2<float>) * mMesh.mTex0Buffer.size(), &(mMesh.mTex0Buffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLTex2Buffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector2<float>) * mMesh.mTex0Buffer.size(), &(mMesh.mTex0Buffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLIndexBuffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(AEuint) * mMesh.mIndexBuffer.size(), &(mMesh.mIndexBuffer[0]), GL_STATIC_DRAW );
}

void Cell::init()
{
	generateIndexBuffer();
	generateNormals();
	generateVertexBuffers();

	aeDelete( mBoundingVolume );
	mBoundingVolume = new ::Mesh::Volume::AABBoxVolume(mSize);
	mBoundingVolume->translation(mSize * 0.5f);
}

void Cell::render()
{
	aePushAttrib(GL_ALL_ATTRIB_BITS);

	//applyFaceCulling(mFacing);
	switch ( mFacing ) {
		case Core::Facing::Both:
			aeForceDisable(GL_CULL_FACE);
			break;
		case Core::Facing::Back:
			aeEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case Core::Facing::Front:
			aeEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
	}

	aeColor(mColor);

	aeBind(mMaterial);

	if ( mShader ) {
		mShader->begin();

		if ( mNormalMap ) {
			mNormalMap->bind();

			aeActiveTexture(GL_TEXTURE1);
			glClientActiveTexture(GL_TEXTURE1);
			aeEnable(GL_TEXTURE_2D, true);

			mShader->set("normalMap", 1);
		}
	}

	if ( mEngineSettings->useMultiTexturing() ) {
		renderMultitextured();
	}
	else {
		renderSingletextured();
	}

	aeUnbind(mMaterial);

	aePopAttrib();
}

void Cell::renderMultitextured()
{
    aeActiveTexture( GL_TEXTURE0 );
    glClientActiveTexture(GL_TEXTURE0);

    glMatrixMode( GL_TEXTURE );
    aePushMatrix();
    glScalef( 32.0f, 32.0f, 1.0f );

    aeForceEnable(GL_TEXTURE_2D);
	aeBind(mBaseTexture);

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, mGLTex0Buffer );
    glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );


	if ( mEngineSettings->useMultiTexturing() ) {
    // Disable lighting because it changes the color of the vertices that are
    // used for the multitexture blending.
    //aeDisable(GL_LIGHTING);


    //
    // Texture Stage 1
    //
    // Perform a linear interpolation between the output of stage 0 
    // (i.e texture0) and texture1 and use the RGB portion of the vertex's 
    // color to mix the two. 
    //
    aeActiveTexture(GL_TEXTURE1);
    glMatrixMode(GL_TEXTURE);
    glScalef( 32.0f, 32.0f, 1.0f );

    aeForceEnable(GL_TEXTURE_2D);
    mAlphaTexture->bind();

    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE );

    glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS );
    glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR );

    glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE );
    glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR );

    glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PRIMARY_COLOR );
    glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_COLOR );

    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, mGLTex1Buffer );
    glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );


    //
    // Texture Stage 2
    //
    // Perform a linear interpolation between the output of stage 1 
    // (i.e texture0 mixed with texture1) and texture2 and use the ALPHA 
    // portion of the vertex's color to mix the two. 
    //
 //   aeActiveTexture(GL_TEXTURE2);
 //   glMatrixMode(GL_TEXTURE);
 //   glScalef( 32.0f, 32.0f, 1.0f );

 //   aeEnable(GL_TEXTURE_2D, true);
	//mAdditionalTexture->bind();

 //   glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
 //   glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE );

 //   glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS );
 //   glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR );

 //   glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE );
 //   glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR );

 //   glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PRIMARY_COLOR );
 //   glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA );

 //   glClientActiveTexture(GL_TEXTURE2);
 //   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 //   glBindBuffer(GL_ARRAY_BUFFER, mGLTex2Buffer);
 //   glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));


/*
	// Shadows
	aeActiveTexture(GL_TEXTURE3);
	aeEnable(GL_TEXTURE_2D, true);
	mBaseTexture->bind();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,	GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,	GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB,	GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB,	GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB,	GL_PRIMARY_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB,	GL_SRC_COLOR);

    glClientActiveTexture(GL_TEXTURE3);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, mGLTex0Buffer);
    glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
*/
	}

	if ( mShader ) {
		mShader->set("colorMap", 1);
	}


    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    glBindBuffer( GL_ARRAY_BUFFER, mGLVertexBuffer );
    glVertexPointer( 3, GL_FLOAT, 0, BUFFER_OFFSET(0) );
    glBindBuffer( GL_ARRAY_BUFFER, mGLColorBuffer );
    glColorPointer( 4, GL_FLOAT, 0, BUFFER_OFFSET(0) );
    glBindBuffer( GL_ARRAY_BUFFER, mGLNormalBuffer );
    glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(0) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLIndexBuffer );
    glDrawElements( GL_TRIANGLES, mMesh.mIndexBuffer.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );


	if ( mShader ) {
		mShader->end();
	}


    aeActiveTexture(GL_TEXTURE3);
    aeForceDisable(GL_TEXTURE_2D);
    glClientActiveTexture(GL_TEXTURE3);
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	aeActiveTexture(GL_TEXTURE2);
    aeForceDisable(GL_TEXTURE_2D);
    glClientActiveTexture(GL_TEXTURE2);
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );

    glActiveTexture(GL_TEXTURE1);
    aeForceDisable(GL_TEXTURE_2D);
    glClientActiveTexture(GL_TEXTURE1);
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	aeActiveTexture(GL_TEXTURE0);
	aePopMatrix();

	glClientActiveTexture( GL_TEXTURE0 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glMatrixMode( GL_MODELVIEW );
}

void Cell::renderSingletextured()
{
    glMatrixMode( GL_TEXTURE );
    aePushMatrix();
    glScalef( 32.0f, 32.0f, 1.0f );

    aeForceEnable(GL_TEXTURE_2D);
	aeBind(mBaseTexture);

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, mGLTex0Buffer );
    glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );


	if ( mShader ) {
		mShader->set("colorMap", 1);
	}


    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    glBindBuffer( GL_ARRAY_BUFFER, mGLVertexBuffer );
    glVertexPointer( 3, GL_FLOAT, 0, BUFFER_OFFSET(0) );
    glBindBuffer( GL_ARRAY_BUFFER, mGLColorBuffer );
    glColorPointer( 4, GL_FLOAT, 0, BUFFER_OFFSET(0) );
    glBindBuffer( GL_ARRAY_BUFFER, mGLNormalBuffer );
    glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(0) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLIndexBuffer );
    glDrawElements( GL_TRIANGLES, mMesh.mIndexBuffer.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );


	if ( mShader ) {
		mShader->end();
	}


	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	aePopMatrix();
}

void Cell::setAdditionalTexture(Texture *additional)
{
	mAdditionalTexture = additional;
}

void Cell::setAlphaTexture(Texture *alpha)
{
	mAlphaTexture = alpha;
}

void Cell::setBaseTexture(Texture *base)
{
	mBaseTexture = base;
}

void Cell::setCellSize(vector2i size)
{
	mCellSize = size;
}

void Cell::setMesh(const Mesh& mesh)
{
	mMesh = mesh;
}

void Cell::unload()
{
	aeDelete(mBoundingVolume);

    aeDeleteBuffer(mGLVertexBuffer);
    aeDeleteBuffer(mGLNormalBuffer);
    aeDeleteBuffer(mGLColorBuffer);
    aeDeleteBuffer(mGLTex0Buffer);
    aeDeleteBuffer(mGLTex1Buffer);
    aeDeleteBuffer(mGLTex2Buffer);
    aeDeleteBuffer(mGLIndexBuffer);
}


}
}
