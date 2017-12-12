
#ifndef _Physics_CollisionGroup_h_
#define _Physics_CollisionGroup_h_


// Library includes
#include <list>

// Project includes
#include <Interfaces/Core/Types.h>

// Forward declarations

// Namespace declarations


namespace Physics {


class CollisionGroup
{
public:
	CollisionGroup();

public:
	// returns all colliders of <ID>
	const Core::IdSet getColliders(Core::AEid id) const;

	void add(Core::AEid id);
	bool contains(Core::AEid id) const;
	void remove(Core::AEid id);

	size_t size() const;

protected:

private:
	Core::IdSet	mIds;
};


typedef std::list<CollisionGroup> CollisionGroupsList;


}


#endif
