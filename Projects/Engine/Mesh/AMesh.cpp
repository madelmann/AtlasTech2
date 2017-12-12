
// Header
#include "AMesh.h"

// Library includes

// Project includes
#include <Mesh/Volume/Volumes.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


AMesh::AMesh(Core::AEid id, const std::string& name)
: Core::RenderObject(id, name),
  mIsAnimated(false),
  mTimeSinceLastAnimation(0.f)
{
}

AMesh::~AMesh()
{
	// hack to fix mem leaks, should be removed/resolved
	aeDelete(mBoundingVolume);
}

void AMesh::addAnimation(const std::string& animation)
{
	mAnimationList.push_back(animation);
}

bool AMesh::animated() const
{
	return mIsAnimated;
}

void AMesh::animated(bool flag)
{
	mIsAnimated = flag;
}

bool AMesh::hasAnimation(const std::string& animation)
{
	for ( StringList::iterator it = mAnimationList.begin(); it != mAnimationList.end(); ++it ) {
		if ( Tools::StringCompareI(animation, (*it)) ) {
			return true;
		}
	}

	return false;
}

void AMesh::setBoundingVolume()
{
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

void AMesh::update(float elapsedTime)
{
	mTimeSinceLastAnimation += elapsedTime;
}


}
