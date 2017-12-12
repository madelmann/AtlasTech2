
// Header
#include "RenderObject.h"

// Library includes
#include <math.h>

// Project includes
#include <Driver/GLWrapper.h>
#include <Interfaces/Core/Types.h>
#include <Interfaces/Driver/IDriver.h>
#include <Interfaces/Driver/IOcclusionQuery.h>
#include <Math/Tools.h>

// Namespace declarations


namespace Core {


RenderObject::RenderObject(Core::AEid id, const std::string& name)
: Object(id, name),
  mAnimationSpeed(100.f),
  mBoundingVolume(0),
  mColor(1.f, 1.f, 1.f),
  mColorMap(0),
  mFacing(Facing::Both),
  mHeightMap(0),
  mMaterial(0),
  mNormalMap(0),
  mOcclusionQuery(0),
  mScale(1.f, 1.f, 1.f),
  mShader(0),
  mTexturePack(0),
  mVolumeType(IVolume::Type::Undefined)
{
}

RenderObject::~RenderObject()
{
}

float RenderObject::animationSpeed() const
{
	return mAnimationSpeed;
}

void RenderObject::animationSpeed(float speed)
{
	mAnimationSpeed = speed;
}

bool RenderObject::collide(vector3f vector, float radius)
{
	switch ( mBoundingVolume->type() ) {
		case IVolume::Type::AABBox:
		case IVolume::Type::Box:
			return boxCollision(vector, radius);
		case IVolume::Type::Cylinder:
			return cylinderCollision(vector, radius);
		case IVolume::Type::Sphere:
			return sphereCollision(vector, radius);
		default:
			return boxCollision(vector, radius);
	}
}

bool RenderObject::boxCollision(vector3f /*vector*/, float /*radius*/, bool /*force*/)
{
	//vector.x -= mPosition.x;
	//vector.y -= mPosition.y;
	//vector.z -= mPosition.z;

	//vector3f size = mBoundingVolume->size();

	//if( vector.x + radius >= 0.f && vector.x - radius <= size.x &&
	//	vector.y + radius >= 0.f && vector.y - radius <= size.y &&
	//	vector.z + radius >= 0.f && vector.z - radius <= size.z ) {
	//	return true;
	//}

	return false;
}

bool RenderObject::cylinderCollision(vector3f /*vector*/, float /*radius*/, bool /*force*/)
{
	//vector.x -= mPosition.x;
	//vector.y -= mPosition.y;
	//vector.z -= mPosition.z;

	//vector3f size = mBoundingVolume->size();

	//if( vector.x + radius >= 0.f && vector.x - radius <= size.x &&
	//	vector.z + radius >= 0.f && vector.z - radius <= size.z )
	//{
	//	if ( vector.y + radius >= 0.f && vector.y - radius <= size.y ) {
	//		return true;
	//	}
	//}

	return false;
}

int RenderObject::deepCollision(vector3f /*vector*/, float /*radius*/, bool /*force*/)
{
	return 0;
}

IVolume* RenderObject::getBoundingVolume() const
{
	return mBoundingVolume;
}

const vector3f& RenderObject::getColor() const
{
	return mColor;
}

TextureSpace::Texture *RenderObject::getColorMap() const
{
	return mColorMap;
}

Facing::E RenderObject::getFacing() const
{
	return mFacing;
}

TextureSpace::Texture* RenderObject::getHeightMap() const
{
	return mHeightMap;
}

MaterialSpace::Material* RenderObject::getMaterial() const
{
	return mMaterial;
}

TextureSpace::Texture* RenderObject::getNormalMap() const
{
	return mNormalMap;
}

IOcclusionQuery* RenderObject::getOcclusionResult()
{
	if ( !mOcclusionQuery ) {
		return 0;
	}

	GLint available;
    glGetQueryObjectivARB(mOcclusionQuery->getId(), GL_QUERY_RESULT_AVAILABLE_ARB, &available);

	if ( available ) {
		return mOcclusionQuery;
	}

	return 0;
}

const vector3f RenderObject::getRotation()
{
	return mRotation;
}

const vector3f& RenderObject::getScale() const
{
	return mScale;
}

const vector3f& RenderObject::getSize() const
{
	return mSize;
}

ShaderSpace::Shader *RenderObject::getShader() const
{
	return mShader;
}

TextureSpace::TexturePack* RenderObject::getTexturePack() const
{
	return mTexturePack;
}

const vector3f& RenderObject::getTranslation() const
{
	return mTranslation;
}

IVolume::Type::E RenderObject::getVolumeType() const
{
	return mVolumeType;
}

void RenderObject::render()
{
	// this method will be implemented by derived classes
}

void RenderObject::renderBoundingVolume(bool solid)
{
	mBoundingVolume->render(solid);
}

void RenderObject::setBoundingVolume(IVolume *volume)
{
	mBoundingVolume = volume;
}

void RenderObject::setColor(const vector3f& color)
{
	mColor = color;
}

void RenderObject::setColorMap(TextureSpace::Texture *colormap)
{
	mColorMap = colormap;
}

void RenderObject::setFacing(Facing::E facing)
{
	mFacing = facing;
}

void RenderObject::setHeightMap(TextureSpace::Texture *heightmap)
{
	mHeightMap = heightmap;
}

void RenderObject::setMaterial(MaterialSpace::Material *material)
{
	mMaterial = material;
}

void RenderObject::setNormalMap(TextureSpace::Texture *normalmap)
{
	mNormalMap = normalmap;
}

void RenderObject::setRotation(const vector3f& rot)
{
	mRotation = rot;
}

void RenderObject::setScale(const vector3f& scale)
{
	mScale = scale;
}

void RenderObject::setSize(const vector3f& size)
{
	mSize = size;
}

void RenderObject::setShader(ShaderSpace::Shader *shader)
{
	mShader = shader;
}

void RenderObject::setTexturePack(TextureSpace::TexturePack *pack)
{
	mTexturePack = pack;
	if ( mTexturePack ) {
		// only override textures if they have been loaded correctly
		if ( mTexturePack->color()  ) { mColorMap = mTexturePack->color();   }
		if ( mTexturePack->height() ) { mHeightMap = mTexturePack->height(); }
		if ( mTexturePack->normal() ) { mNormalMap = mTexturePack->normal(); }
	}
}

void RenderObject::setTranslation(const vector3f& trans)
{
	mTranslation = trans;
}

void RenderObject::setVolumeType(IVolume::Type::E type)
{
	mVolumeType = type;
}

bool RenderObject::sphereCollision(vector3f /*vector*/, float /*radius*/, bool /*force*/)
{
// not correct
	//vector.x -= mPosition.x;
	//vector.y -= mPosition.y;
	//vector.z -= mPosition.z;

	//vector3f size = mBoundingVolume->size();

	//if( vector.x + radius >= 0.f && vector.x - radius <= size.x &&
	//	vector.z + radius >= 0.f && vector.z - radius <= size.z )
	//{
	//	if ( vector.y + radius >= 0.f && vector.y - radius <= size.y ) {
	//		return true;
	//	}
	//}

	return false;
}

void RenderObject::updateOcclusionQuery(IDriver *driver)
{
	assert(driver);
	assert(driver->getCurrentOcclusionQuery() == 0);

	if ( !mOcclusionQuery ) {
		mOcclusionQuery = driver->createOcclusionQuery();

		if ( !mOcclusionQuery ) {
			assert(!"could not create occlusion query!");
			return;
		}
	}

	mOcclusionQuery->begin();
		aePushMatrix();
		renderBoundingVolume(true);
		aePopMatrix();
	mOcclusionQuery->end();
}


}
