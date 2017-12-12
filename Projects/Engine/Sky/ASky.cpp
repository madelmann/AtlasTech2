
// Header
#include "ASky.h"

// Library includes

// Project includes

// Namespace declarations


namespace SkySpace {


ASky::ASky(Core::AEid id)
: Mesh::AMesh(id, "Sky"),
  mMeshId(0),
  mSettings(0)
{
}

ASky::~ASky()
{
	unload();
}

void ASky::connectSettings(Settings *s)
{
	mSettings = s;
}

void ASky::init()
{
	mSize = mSettings->size();

	preprocess();
	setBoundingVolume();
}


}
