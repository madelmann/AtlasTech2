
// Header
#include "ParticleSystem.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Mesh/Volume/Volumes.h>
#include <Mesh/Utils.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Mesh {


ParticleSystem::ParticleSystem(Core::AEid id, const std::string& name)
: AMesh(id, name),
  mResizeBoundingVolume(true)
{
}

ParticleSystem::~ParticleSystem()
{
	unload();
}

void ParticleSystem::addEmitter(Emitter e)
{
	mEmitters.push_back(e);

	if ( mResizeBoundingVolume ) {
		Core::RenderObject *ro = e.mesh();
		if ( !ro ) {
			return;
		}

		IVolume *v = ro->getBoundingVolume();
		if ( !v ) {
			return;
		}

		//vector3f pos = e.position();
		//if ( pos.x - v->size().x < mBoundingVolume->position() - mBoundingVolume->size().x ) {

		//}
	}
}

void ParticleSystem::init()
{
	setBoundingVolume();
}

void ParticleSystem::render()
{
	aeTranslate(getTranslation());
	aeRotate(getRotation());

	aePushAttrib(GL_CULL_FACE);
	//aePushAttrib(GL_LIGHTING);
	aePushAttrib(GL_TEXTURE_2D);

		applyFaceCulling(mFacing);

		for ( EmitterList::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it ) {
			aePushMatrix();
				aeTranslate((*it).position());

				(*it).render();
			aePopMatrix();
		}

	aePopAttrib();
	//aePopAttrib();
	aePopAttrib();
}

void ParticleSystem::setBoundingVolume()
{
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

void ParticleSystem::unload()
{
	//for ( EmitterList::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it ) {
	//	delete (*it);
	//}
	mEmitters.clear();

	aeDelete(mBoundingVolume);
}


}
