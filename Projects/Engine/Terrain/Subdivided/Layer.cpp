
// Header
#include "Layer.h"

// Library includes

// Project includes
#include "Cell.h"
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Mesh/Volume/AABBox.h>
#include <Scene/SceneGraphObject.h>
#include <Texture/Loader/TGA/TGA.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Terrain {
namespace Subdivided {


Layer::Layer(const Common::Logger *p)
: Common::Logger(p, "Layer")
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

	bool result = load();

	subdivide();

	return result;
}

void Layer::clear()
{
	debug("Clearing...");

	aeDelete(mBoundingVolume);

	for ( CellList::iterator it = mCells.begin(); it != mCells.end(); it++ ) {
		delete (*it);
	}
	mCells.clear();
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
		warn("could not load terrain file '" + file + "'!");
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

	mBoundingVolume = new Mesh::Volume::AABBoxVolume(
		//vector3f(mMapSize.x * mScale.x, maxHeight - minHeight, mMapSize.y * mScale.z)
		vector3f()
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

	// Multiply the position by the inverse of the terrain matrix 
	// to get the position in terrain local space
	vector3f terrainPos = pos;
	vector3f invBlockScale( 1.f / mScale.x, 0.f, 1.f / mScale.z );

	// Get the 4 vertices that make up the triangle we're over
	vector3f vertexIndices = terrainPos * invBlockScale;

	int u0 = (int)floorf(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;

	if ( u0 >= 0 && u1 < mMapSize.x && v0 >= 0 && v1 < mMapSize.y ) {                    
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
	// nothing to render here (this is all done in our scene graph)
}

void Layer::subdivide()
{
	assert( mPositionBuffer.size() == mColorBuffer.size() );
	assert( mPositionBuffer.size() == mNormalBuffer.size() );


	vector2i vpc;		// vertices per cell
	vpc.x = int(mMapSize.x / mSubdivisions.x);
	vpc.y = int(mMapSize.y / mSubdivisions.y);

	int numVerts = (vpc.x + 1) * (vpc.y + 1);

	for ( int sx = 0; sx < (int)mSubdivisions.x; sx++ ) {
		for ( int sz = 0; sz < (int)mSubdivisions.y; sz++ ) {
			Cell *cell = new Cell();

			cell->connectEngineSettings(mEngineSettings);
			cell->setAdditionalTexture(mAdditionalTexture);
			cell->setAlphaTexture(mAlphaTexture);
			cell->setBaseTexture(mBaseTexture);
			cell->setCellSize(vector2i(vpc.x + 1, vpc.y + 1));

			Cell::Mesh mesh;
			mesh.mColorBuffer.resize(numVerts);
			mesh.mNormalBuffer.resize(numVerts);
			mesh.mPositionBuffer.resize(numVerts);
			mesh.mTex0Buffer.resize(numVerts);

			float maxHeight = 0.f;
			float minHeight = 255.f * mScale.y;
			vector3f pos(sx * vpc.x * mScale.x, 0.f, sz * vpc.y * mScale.z);
			vector3f size;

			int localIdx = 0;
			for ( int x = sx * vpc.x; x < (sx + 1) * vpc.x + 1; x++ ) {
				for ( int z = sz * vpc.y; z < (sz + 1) * vpc.y + 1; z++ ) {
					int globalIdx = z * mMapSize.x + x;

					if ( maxHeight < mPositionBuffer[globalIdx].y ) {
						maxHeight = mPositionBuffer[globalIdx].y;
					}
					if ( minHeight > mPositionBuffer[globalIdx].y ) {
						minHeight = mPositionBuffer[globalIdx].y;
					}

					mesh.mColorBuffer[localIdx] = mColorBuffer[globalIdx];
					mesh.mNormalBuffer[localIdx] = mNormalBuffer[globalIdx];
					mesh.mPositionBuffer[localIdx] = size = mPositionBuffer[globalIdx] - pos;
					mesh.mTex0Buffer[localIdx] = mTex0Buffer[globalIdx];

					localIdx++;
				}
			}

			//pos.y = mOffset + minHeight;
			size.y = 255.f * mScale.y;	//maxHeight - minHeight;

			cell->setMesh(mesh);
			cell->setSize(size);
			cell->init();

			mCells.push_back(cell);

			Scene::SceneGraphObject *sgo = mSceneManager->create(cell);
			sgo->setPosition(pos);
			mSceneManager->attach(sgo, Core::invalidID, true);
		}
	}
}


}
}
