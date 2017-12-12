
// Header
#include "AVolumeMesh.h"

// Library includes

// Project includes

// Namespace declarations


namespace Mesh {


AVolumeMesh::AVolumeMesh(const vector3f& size)
{
	setSize(size);
}

AVolumeMesh::~AVolumeMesh()
{
	unload();
}

void AVolumeMesh::render()
{
	aeTranslate(getTranslation());
	aeRotate(getRotation());

	renderBoundingVolume(true);
}

void AVolumeMesh::resize(const vector3f& size)
{
	mSize = size;

	init();
}

void AVolumeMesh::setAnimation(const std::string& /*animation*/)
{
}

void AVolumeMesh::unload()
{
	aeDelete( mBoundingVolume );
}

void AVolumeMesh::update(float /*elapsedTime*/)
{
}


}
