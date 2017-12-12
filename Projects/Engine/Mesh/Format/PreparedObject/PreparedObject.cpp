
// Header
#include "PreparedObject.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Mesh/Volume/Volumes.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


PreparedObject::PreparedObject(Core::AEid id, const std::string& name)
: AMesh(id, name),
  mResizeBoundingVolume(true)
{
}

PreparedObject::~PreparedObject()
{
	unload();
}

void PreparedObject::addLeaf(Leaf l)
{
	mLeaves.push_back(l);

	if ( mResizeBoundingVolume ) {
		Core::RenderObject *ro = l.mesh();
		if ( !ro ) {
			return;
		}

		addVolumeToSize(ro->getBoundingVolume(), l.position());
	}
}

void PreparedObject::addVolumeToSize(IVolume *v, const vector3f& position)
{
	if ( !v ) {
		return;
	}

	vector3f mySize = mChangedSize;
	vector3f newSize = v->size() * 0.5f;

	vector3f delta;
	if ( position.x - newSize.x < 0.f ) {
		delta.x += abs(position.x - newSize.x);
	}
	if ( position.x + newSize.x > mySize.x ) {
		delta.x += abs(position.x + newSize.x - mySize.x);
	}
	if ( position.y - newSize.y < 0.f ) {
		delta.y += abs(position.y - newSize.y);
	}
	if ( position.y + newSize.y > mySize.y ) {
		delta.y += abs(position.y + newSize.y - mySize.y);
	}
	if ( position.z - newSize.z < 0.f ) {
		delta.z += abs(position.z - newSize.z);
	}
	if ( position.z + newSize.z > mySize.z ) {
		delta.z += abs(position.z + newSize.z - mySize.z);
	}

	mChangedSize += delta;
	// TODO: adopt position according to size change
	// Problem: position has to be set in SceneGraphObject => change translation instead?
	mTranslation += delta * 0.5f;
}

void PreparedObject::init()
{
	setBoundingVolume();
}

void PreparedObject::render()
{
	for ( Leaves::iterator it = mLeaves.begin(); it != mLeaves.end(); ++it ) {
		aePushMatrix();
			aeTranslate((*it).position());

			(*it).render();
		aePopMatrix();
	}
}

void PreparedObject::setBoundingVolume()
{
	if ( mResizeBoundingVolume ) {
		mSize = mChangedSize;
	}

	vector3f size = mSize * mScale;

	aeDelete( mBoundingVolume );
	switch ( mVolumeType ) {
		case IVolume::Type::AABBox:
		case IVolume::Type::AABBoxNoTranslate:
			mBoundingVolume = new Mesh::Volume::AABBoxVolume(size);
			break;
		case IVolume::Type::Box:
			mBoundingVolume = new Mesh::Volume::BoxVolume(size);
			break;
		case IVolume::Type::Cylinder:
			mBoundingVolume = new Mesh::Volume::CylinderVolume(size);
			break;
		case IVolume::Type::Sphere:
			mBoundingVolume = new Mesh::Volume::SphereVolume(size);
			break;
		default:
			assert(!"no default volume set!");
			break;
	}
}

void PreparedObject::unload()
{
	mLeaves.clear();

	aeDelete(mBoundingVolume);
}


}
