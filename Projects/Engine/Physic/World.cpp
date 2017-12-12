
// Header
#include "World.h"

// Library includes

// Project includes

// Namespace declarations


namespace Physics {


World::World(const vector3f& position, const vector3f& size)
: mPosition(position),
  mSize(size),
  mStartId(0)
{
}

const vector3f& World::position() const
{
	return mPosition;
}

const vector3f& World::size() const
{
	return mSize;
}

void World::subdivide(const vector3f& subdivisions)
{
	// subdivide the world in several smaller sized containers
	// according to the given number of subdivisions

	vector3f csize(mSize.x / (int)subdivisions.x, mSize.y / (int)subdivisions.y, mSize.z / (int)subdivisions.z);

	int x = 0;
	int y = 0;
	int z = 0;

	for ( x = 0; x < subdivisions.x; ++x ) {
		for ( y = 0; y < subdivisions.y; ++y ) {
			for ( z = 0; z < subdivisions.z; ++z ) {
				vector3f topLeftFront(
					mPosition.x + x * csize.x,
					mPosition.y + y * csize.y,
					mPosition.z + z * csize.z
				);
				vector3f bottomRightBack(
					mPosition.x + (x + 1) * csize.x,
					mPosition.y + (y + 1) * csize.y,
					mPosition.z + (z + 1) * csize.z
				);

				Container c(mStartId++, topLeftFront, bottomRightBack);
				mContainers[mStartId] = c;
			}
		}
	}
}


}
