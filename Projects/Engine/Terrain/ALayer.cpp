
// Header
#include "ALayer.h"

// Library includes

// Project includes
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Math/Normals.h>
#include <Math/Tools.h>
#include <Mesh/Volume/AABBox.h>
#include <Mesh/Volume/Cylinder.h>
#include <Mesh/Volume/Sphere.h>
#include <Shader/Shader.h>
#include <Scene/SceneGraphObject.h>
#include <Texture/Texture.h>

// Namespace decalrations


namespace Terrain {


ALayer::ALayer()
: mAdditionalTexture(0),
  mAlphaTexture(0),
  mBaseTexture(0),
  mEngineSettings(0),
  mEnabled(true),		// set to default = false after updating all maps
  mMediaPathManager(0),
  mRenderNormals(false),
  mSceneManager(0),
  mSettings(0)
{
}

ALayer::~ALayer()
{
}

void ALayer::addTexture(const TextureSettings &texture)
{
	mTextures.push_back(texture);
}

void ALayer::connectEngineSettings(Core::EngineSettings *s)
{
	assert(s);
	assert(!mEngineSettings);

	mEngineSettings = s;
}

void ALayer::connectMediaPathManager(IMediaPathManager *m)
{
	assert(m);
	assert(!mMediaPathManager);

	mMediaPathManager = m;
}

void ALayer::connectSceneManager(Scene::IManager *m)
{
	assert(m);
	assert(!mSceneManager);

	mSceneManager = m;
}

void ALayer::connectSettings(Settings *s)
{
	assert(s);
	assert(!mSettings);

	mSettings = s;
}

bool ALayer::enabled() const
{
	return mEnabled;
}

void ALayer::enabled(bool state)
{
	mEnabled = state;
}

int ALayer::height() const
{
	return mMapSize.y;
}

void ALayer::setAdditionalTexture(Texture *additional)
{
	mAdditionalTexture = additional;
}

void ALayer::setAlphaTexture(Texture *alpha)
{
	mAlphaTexture = alpha;
}

void ALayer::setBaseTexture(Texture *base)
{
	mBaseTexture = base;
}

void ALayer::setHeightMap(const std::string &heightmap)
{
	mHeightMap = heightmap;
}

void ALayer::setOffset(float offset)
{
	mOffset = offset;
}

void ALayer::setSubdivisions(const vector2f& sub)
{
	mSubdivisions = sub;
}

int ALayer::width() const
{
	return mMapSize.x;
}


}
