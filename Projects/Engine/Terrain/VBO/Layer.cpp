
// Header
#include "Layer.h"

// Library includes
#ifdef USE_BULLET
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#endif

// Project includes
#include <Driver/GLWrapper.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Physics/IManager.h>
#include <Interfaces/Physics/Types.h>
#include <Interfaces/Scene/IManager.h>
#include <Math/Normals.h>
#include <Math/Tools.h>
#include <Material/Material.h>
#include <Mesh/Volume/AABBox.h>
#ifdef USE_BULLET
#include <Physic/PhysicalObject.h>
#endif
#include <Shader/Shader.h>
#include <Scene/SceneGraphObject.h>
#include <Texture/Loader/TGA/TGA.h>
#include <Texture/Texture.h>

// Namespace declarations


#define BUFFER_OFFSET(i) ((char*)NULL + (i))


namespace Terrain {
namespace VBO {


//static btVector3 getUpVector(int upAxis, btScalar regularValue, btScalar upValue)
//{
//	btAssert(upAxis >= 0 && upAxis <= 2 && "bad up axis");
//
//	btVector3 v(regularValue, regularValue, regularValue);
//	v[upAxis] = upValue;
//
//	return v;
//}



Layer::Layer(const Common::Logger *p)
: Common::Logger(p, "Layer"),
  mGLColorBuffer(0),
  mGLIndexBuffer(0),
  mGLNormalBuffer(0),
  mGLTex0Buffer(0),
  mGLTex1Buffer(0),
  mGLTex2Buffer(0),
  mGLVertexBuffer(0),
  mHeightfieldShape(0),
  mPhysicManager(0),
  mPhysicalObject(0)
{
}

Layer::~Layer()
{
	clear();
}

bool Layer::build()
{
	if ( !enabled() ) {
		return false;
	}

	assert(mPhysicManager);

	return load();
}

void Layer::clear()
{
	debug("Clearing...");

	aeDelete(mBoundingVolume);
	aeDelete(mRawHeightfieldData);

    aeDeleteBuffer(mGLVertexBuffer);
    aeDeleteBuffer(mGLNormalBuffer);
    aeDeleteBuffer(mGLColorBuffer);
    aeDeleteBuffer(mGLTex0Buffer);
    aeDeleteBuffer(mGLTex1Buffer);
    aeDeleteBuffer(mGLTex2Buffer);
    aeDeleteBuffer(mGLIndexBuffer);
}

void Layer::connectPhysicManager(Physics::IManager *m)
{
	assert(m);
	assert(!mPhysicManager);

	mPhysicManager = m;
}

void Layer::generateCollisionMap()
{
#ifdef USE_BULLET
	mHeightfieldShape = new btHeightfieldTerrainShape(
		mMapSize.x,
		mMapSize.y,
		mRawHeightfieldData,
		mScale.y,
		100.f,
		400.f,
		1,
		PHY_FLOAT,
		false
	);

	// scale the shape
	btVector3 localScaling(mScale.x, mScale.y, mScale.z);
	mHeightfieldShape->setLocalScaling(localScaling);
	mHeightfieldShape->setUseDiamondSubdivision(true);

	// set origin to middle of heightfield
	//btTransform tr;
	//tr.setIdentity();
	//tr.setOrigin(btVector3(0, -20, 0));


	mPhysicalObject = mPhysicManager->create(0.f, mSettings->size(), mHeightfieldShape);
	mPhysicalObject->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
	mPhysicalObject->setCollisionGroup(Physics::CollisionType::All);
	mPhysicalObject->setCollisionMask(Physics::CollideWithAll);
	mPhysicalObject->setMotionState(vector3f(0.f, -1.f, 0.f), vector3f());
	mPhysicalObject->init(vector3f(), vector3f());
#endif
}

void Layer::generateIndexBuffer()
{
    if ( mMapSize.x < 2 || mMapSize.y < 2 ) {
        // Terrain hasn't been loaded, or is of an incorrect size
        return;
    }

    const unsigned int terrainWidth = mMapSize.x;
    const unsigned int terrainHeight = mMapSize.y;

    // 2 triangles for every quad of the terrain mesh
    const unsigned int numTriangles = ( terrainWidth - 1 ) * ( terrainHeight - 1 ) * 2;

    // 3 indices for each triangle in the terrain mesh
    mIndexBuffer.resize( numTriangles * 3 );

    unsigned int index = 0; // Index in the index buffer
    for ( unsigned int j = 0; j < (terrainHeight - 1); ++j ) {
        for ( unsigned int i = 0; i < (terrainWidth - 1); ++i ) {
            int vertexIndex = ( j * terrainWidth ) + i;
            // Top triangle (T0)
            mIndexBuffer[index++] = vertexIndex;                           // V0
            mIndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
            mIndexBuffer[index++] = vertexIndex + 1;                       // V1
            // Bottom triangle (T1)
            mIndexBuffer[index++] = vertexIndex;                           // V0
            mIndexBuffer[index++] = vertexIndex + terrainWidth;            // V2
            mIndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
        }
    }
}

void Layer::generateNormals()
{
    for ( size_t i = 0; i < mIndexBuffer.size(); i += 3 ) {
        vector3f v0 = mPositionBuffer[ mIndexBuffer[i + 0] ];
        vector3f v1 = mPositionBuffer[ mIndexBuffer[i + 1] ];
        vector3f v2 = mPositionBuffer[ mIndexBuffer[i + 2] ];

		vector3f normal = normalize( cross( v1 - v0, v2 - v0 ) );

        mNormalBuffer[ mIndexBuffer[i + 0] ] += normal;
        mNormalBuffer[ mIndexBuffer[i + 1] ] += normal;
        mNormalBuffer[ mIndexBuffer[i + 2] ] += normal;
    }

    const vector3f UP( 0.0f, 1.0f, 0.0f );
    for ( size_t i = 0; i < mNormalBuffer.size(); ++i ) {
        mNormalBuffer[i] = normalize( mNormalBuffer[i] );

#if ENABLE_SLOPE_BASED_BLEND
        float fTexture0Contribution = saturate( dot( mNormalBuffer[i], UP ) - 0.1f );
        mColorBuffer[i] = vector4<float>( fTexture0Contribution, fTexture0Contribution, fTexture0Contribution, mColorBuffer[i].w );
#endif
    }
}

void Layer::generateVertexBuffers()
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
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector3f) * mPositionBuffer.size(), &(mPositionBuffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLColorBuffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector4<float>) * mColorBuffer.size(), &(mColorBuffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLNormalBuffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector3f) * mNormalBuffer.size(), &(mNormalBuffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLTex0Buffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector2<float>) * mTex0Buffer.size(), &(mTex0Buffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLTex1Buffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector2<float>) * mTex0Buffer.size(), &(mTex0Buffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, mGLTex2Buffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(vector2<float>) * mTex0Buffer.size(), &(mTex0Buffer[0]), GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLIndexBuffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW );	// emtpy buffer
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(AEuint) * mIndexBuffer.size(), &(mIndexBuffer[0]), GL_STATIC_DRAW );
}

vector3f Layer::heightAt(const vector3f& pos)
{
	vector3f result(pos.x, 0.f, pos.z);

	// Check if the terrain dimensions are valid
	if ( mMapSize.x < 2 || mMapSize.y < 2 ) {
		return result;
	}

	// Multiple the position by the inverse of the terrain matrix 
	// to get the position in terrain local space
	vector3f terrainPos = vector3f(pos.x, pos.y, pos.z);
	vector3f invBlockScale( 1.f / mScale.x, 0.f, 1.f / mScale.z );

	// Get the 4 vertices that make up the triangle we're over
	vector3f vertexIndices = terrainPos * invBlockScale;

	int u0 = (int)floorf(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;

	if ( u0 >= 0 && u1 < mMapSize.x && v0 >= 0 && v1 < mMapSize.y ) {                    
		vector3f p00 = mPositionBuffer[ ( v0 * mMapSize.x ) + u0 ];    // Top-left vertex
		vector3f p10 = mPositionBuffer[ ( v0 * mMapSize.x ) + u1 ];    // Top-right vertex
		vector3f p01 = mPositionBuffer[ ( v1 * mMapSize.x ) + u0 ];    // Bottom-left vertex
		vector3f p11 = mPositionBuffer[ ( v1 * mMapSize.x ) + u1 ];    // Bottom-right vertex

		// Which triangle are we over?
		float percentU = vertexIndices.x - u0;
		float percentV = vertexIndices.z - v0;

		vector3f dU, dV;
		if ( percentU > percentV ) {   // Top triangle
			dU = p10 - p00;
			dV = p11 - p10;
		}
		else {   // Bottom triangle
			dU = p11 - p01;
			dV = p01 - p00;
		}

		vector3f heightPos = p00 + ( dU * percentU ) + ( dV * percentV );
		result.y = heightPos.y;
	}

	return result;
}

bool Layer::load()
{
	std::string file = mMediaPathManager->getTerrainPath() + mHeightMap;

	// Load our terrain data from file
	TextureSpace::TGAImg terrain;
	if ( terrain.load(file.c_str()) != IMG_OK ) {
		warn("could not load terrain from file '" + file + "'!");
		return false;
	}

	int iBPP = terrain.getBPP() / 8;
	if ( iBPP != 4 ) {
		warn("invalid color depth!");
		return false;
	}

	mMapSize.x = terrain.getWidth();
	mMapSize.y = terrain.getHeight();
	mSettings->size(vector3f(mScale.x * width(), 0.f, mScale.z * height()));

	if ( ((mMapSize.x - 2) % 16 != 0) || ((mMapSize.y - 2) % 16 != 0) ) {
		warn("invalid terrain size!");
		return false;
	}

    unsigned int numVerts = mMapSize.x * mMapSize.y;
    mColorBuffer.resize( numVerts );
    mNormalBuffer.resize( numVerts );
    mPositionBuffer.resize( numVerts );
    mTex0Buffer.resize( numVerts );

	unsigned char* pImage = terrain.getImg();

	// Process terrain data
	int d = 0, x = 0, z = 0;

	float maxHeight = 0.f;
	float minHeight = 255.f * mScale.y;

	mRawHeightfieldData = new float[numVerts * sizeof(float)];

	for ( unsigned int i = 0; i < numVerts; i += 1 ) {
		z = i / mMapSize.y;
		x = i - z * mMapSize.x;
		d = i * iBPP;

		int value;

		// Normals
		value = pImage[d++];
		mNormalBuffer[i].x = float(value) / 255.0f;
		value = pImage[d++];
		mNormalBuffer[i].y = float(value) / 255.0f;
		value = pImage[d++];
		mNormalBuffer[i].z = float(value) / 255.0f;
		// Heightmap
		value = pImage[d++];
		mPositionBuffer[i].x = x * mScale.x;
		mPositionBuffer[i].y = float(value) * mScale.y + mOffset;
		mPositionBuffer[i].z = z * mScale.z;

		mRawHeightfieldData[i] = mPositionBuffer[i].y;

		if ( maxHeight < mPositionBuffer[i].y ) {
			maxHeight = mPositionBuffer[i].y;
		}
		if ( minHeight > mPositionBuffer[i].y ) {
			minHeight = mPositionBuffer[i].y;
		}

		float S = ( x / (float)(mMapSize.x - 1) );
		float T = ( z / (float)(mMapSize.y - 1) );

		mTex0Buffer[i] = vector2<float>( S, T );

		if ( mEngineSettings->useMultiTexturing() ) {
			// Blend 3 textures depending on the height of the terrain
			float tex0Contribution = 1.0f - (mPositionBuffer[i].y / 255.f);
			float tex2Contribution = 0.75f - (mPositionBuffer[i].y / 255.f);

			mColorBuffer[i] = vector4<float>( tex0Contribution, tex0Contribution, tex0Contribution, tex2Contribution );
		}
		else {
			mColorBuffer[i] = vector4<float>(1.f, 1.f, 1.f, 1.f);
		}
	}

    generateIndexBuffer();
    generateNormals();
    generateVertexBuffers();
	generateCollisionMap();

	mBoundingVolume = new Mesh::Volume::AABBoxVolume(
		vector3f(mMapSize.x * mScale.x, maxHeight - minHeight, mMapSize.y * mScale.z)
	);

	return true;
}

vector3f Layer::normalAt(const vector3f& pos)
{
	vector3f result(1.f, 1.f, 1.f);

	// Check if the terrain dimensions are valid
	if ( mMapSize.x < 2 || mMapSize.y < 2 ) {
		return result;
	}

	// Multiple the position by the inverse of the terrain matrix 
	// to get the position in terrain local space
	vector3f terrainPos = pos;
	vector3f invBlockScale( 1.f / mScale.x, 0.f, 1.f / mScale.z );

	// Get the 4 vertices that make up the triangle we're over
	vector3f vertexIndices = terrainPos * invBlockScale;

	int u0 = (int)floorf(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;

	if ( u0 >= 0 && u1 < (int)mMapSize.x && v0 >= 0 && v1 < (int)mMapSize.y ) {                    
		vector3f p00 = mNormalBuffer[ ( v0 * mMapSize.x ) + u0 ];    // Top-left vertex
		vector3f p10 = mNormalBuffer[ ( v0 * mMapSize.x ) + u1 ];    // Top-right vertex
		vector3f p01 = mNormalBuffer[ ( v1 * mMapSize.x ) + u0 ];    // Bottom-left vertex
		vector3f p11 = mNormalBuffer[ ( v1 * mMapSize.x ) + u1 ];    // Bottom-right vertex

		// Which triangle are we over?
		float percentU = vertexIndices.x - u0;
		float percentV = vertexIndices.z - v0;

		vector3f dU, dV;
		if ( percentU > percentV ) {   // Top triangle
			dU = p10 - p00;
			dV = p11 - p10;
		}
		else {   // Bottom triangle
			dU = p11 - p01;
			dV = p01 - p00;
		}

		result = p00 + ( dU * percentU ) + ( dV * percentV );
	}

	result.normalize();
	return result;
}

void Layer::render()
{
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

	if ( mMaterial ) {
		mMaterial->bind();
	}

/*
	if ( mShader && mNormalMap ) {
		mShader->begin();

		mNormalMap->bind();

		aeActiveTexture(GL_TEXTURE1);
		aeEnable(GL_TEXTURE_2D, true);

		mShader->set("normalMap", 1);
	}
*/

	if ( mEngineSettings->useMultiTexturing() ) {
		renderMultitextured();
	}
	else {
		renderSingletextured();
	}

/*
	if ( mShader ) {
		mShader->end();
	}
*/

	if ( mSettings->renderNormals() ) {
		renderNormals();
	}
}

void Layer::renderMultitextured()
{
    aeActiveTexture( GL_TEXTURE0 );
    glClientActiveTexture(GL_TEXTURE0);

    glMatrixMode( GL_TEXTURE );
    aePushMatrix();
		glScalef( 32.0f, 32.0f , 1.0f );

		aeEnable(GL_TEXTURE_2D, true);
		mBaseTexture->bind();

		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glBindBuffer( GL_ARRAY_BUFFER, mGLTex0Buffer );
		glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );

		// Disable lighting because it changes the color of the vertices that are
		// used for the multitexture blending.
		//aeDisable(GL_LIGHTING, true);

/*
		//
		// Texture Stage 1
		//
		// Perform a linear interpolation between the output of stage 0 
		// (i.e texture0) and texture1 and use the RGB portion of the vertex's 
		// color to mix the two. 
		//
		aeActiveTexture(GL_TEXTURE1);
		glClientActiveTexture(GL_TEXTURE1);
		glMatrixMode(GL_TEXTURE);
		glScalef( 32.0f, 32.0f , 1.0f );

		aeEnable(GL_TEXTURE_2D, true);
		mAlphaTexture->bind();

		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
		glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE );

		glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS );
		glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR );

		glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE );
		glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR );

		glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PRIMARY_COLOR );
		glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_COLOR );

		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glBindBuffer( GL_ARRAY_BUFFER, mGLTex1Buffer );
		glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );
*/
/*
		//
		// Texture Stage 2
		//
		// Perform a linear interpolation between the output of stage 1 
		// (i.e texture0 mixed with texture1) and texture2 and use the ALPHA 
		// portion of the vertex's color to mix the two. 
		//
		aeActiveTexture(GL_TEXTURE2);
		glMatrixMode(GL_TEXTURE);
		glScalef( 32.0f, 32.0f , 1.0f );

		aeEnable(GL_TEXTURE_2D, true);
		mAdditionalTexture->bind();

		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
		glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE );

		glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS );
		glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR );

		glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE );
		glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR );

		glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PRIMARY_COLOR );
		glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA );

		glClientActiveTexture(GL_TEXTURE2);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mGLTex2Buffer);
		glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
*/
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

/*
		if ( mShader ) {
			mShader->set("colorMap", 1);
		}
*/
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
		glDrawElements( GL_TRIANGLES, mIndexBuffer.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );

	/*
		aeActiveTexture(GL_TEXTURE3);
		aeDisable(GL_TEXTURE_2D, true);
		glClientActiveTexture(GL_TEXTURE3);
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
*/
/*
		aeActiveTexture(GL_TEXTURE2);
		aeDisable(GL_TEXTURE_2D, true);
		glClientActiveTexture(GL_TEXTURE2);
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
*/
/*
		glActiveTexture(GL_TEXTURE1);
		aeDisable(GL_TEXTURE_2D, true);
		glClientActiveTexture(GL_TEXTURE1);
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
*/
	aePopMatrix();

	aeActiveTexture( GL_TEXTURE0 );
	glClientActiveTexture( GL_TEXTURE0 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glMatrixMode( GL_MODELVIEW );
}

void Layer::renderNormals()
{
    aePushAttrib(GL_ENABLE_BIT);
    aeDisable(GL_LIGHTING);

    aeColor3f(1.0f, 1.0f, 0.0f);	// Yellow

	glLineWidth(2.f);

    aeBegin(GL_LINES);
        for ( size_t i = 0; i < mPositionBuffer.size(); ++i ) {
            vector3f p0 = mPositionBuffer[i];
            vector3f p1 = mPositionBuffer[i] + mNormalBuffer[i] * 3.33f;

			aeColor(mNormalBuffer[i]);
            aeVertex3v( p0 );
            aeVertex3v( p1 );
        }
    aeEnd();

	glLineWidth(1.f);

    aeEnable(GL_LIGHTING);
    aePopAttrib();
}

void Layer::renderSingletextured()
{
    aeActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);

    glMatrixMode(GL_TEXTURE);
    aePushMatrix();
    glScalef( 32.0f, 32.0f, 1.0f );

    aeEnable(GL_TEXTURE_2D);
	mBaseTexture->bind();

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
    glDrawElements( GL_TRIANGLES, mIndexBuffer.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );

	aePopMatrix();

	glClientActiveTexture( GL_TEXTURE0 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glMatrixMode( GL_MODELVIEW );
}


}
}
