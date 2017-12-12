
#ifndef _Physics_World_h_
#define _Physics_World_h_


// Library includes
#include <list>
#include <map>

// Project includes
#include <Interfaces/Core/Id.h>
#include <Interfaces/Core/Types.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Physics {


// Represents the physical world which "contains" all the physical objects
// Will be subdivided in equally sized parts similar to an octree
class World
{
public:
	// Axis aligned "container" represented by an id
	class Container : public Core::Id
	{
	public:
		Container()
		{ }
		Container(Core::AEid id, const vector3f& topLeftFront, const vector3f& bottomRightBack)
		: Core::Id(id),
		  mBottomRightBack(bottomRightBack),
		  mTopLeftFront(topLeftFront)
		{ }

		// adds an object id to our list
		void add(Core::AEid id) {
			mContainedObjects.push_back(id);
		}

		// clears all contained ids
		void clear() {
			mContainedObjects.clear();
		}

		// exports a copy of our id list
		void exportObjects(Core::IdList &list) {
			list = mContainedObjects;
		}

		// check if a position is inside of our container
		bool contains(const vector3f& position) const {
			return (position.x >= mTopLeftFront.x && position.y && position.y <= mTopLeftFront.y && position.z >= mTopLeftFront.z)
				&& (position.x <= mBottomRightBack.x && position.y >= mBottomRightBack.y && position.z <= mBottomRightBack.z);
		}

	private:
		vector3f		mBottomRightBack;
		Core::IdList	mContainedObjects;
		vector3f		mTopLeftFront;
	};

public:
	World(const vector3f& position, const vector3f& size);

public:
	const vector3f& position() const;
	const vector3f& size() const;

	void subdivide(const vector3f& subdivisions);

protected:

private:
	typedef std::map<Core::AEid, Container> ContainerMap;

private:
	ContainerMap	mContainers;
	vector3f		mPosition;
	vector3f		mSize;
	Core::AEid		mStartId;
};


}


#endif
