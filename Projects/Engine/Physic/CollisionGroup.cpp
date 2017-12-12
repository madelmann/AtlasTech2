
// Header
#include "CollisionGroup.h"

// Library includes

// Project includes

// Namespace declarations


namespace Physics {


CollisionGroup::CollisionGroup()
{
}

void CollisionGroup::add(Core::AEid id)
{
	mIds.insert(id);
}

bool CollisionGroup::contains(Core::AEid id) const
{
	return (mIds.find(id) != mIds.end());
}

const Core::IdSet CollisionGroup::getColliders(Core::AEid id) const
{
	Core::IdSet result;

	for ( Core::IdSet::const_iterator it = mIds.begin(); it != mIds.end(); ++it ) {
		if ( (*it) != id ) {
			result.insert((*it));
		}
	}

	return result;
}

void CollisionGroup::remove(Core::AEid id)
{
	Core::IdSet::iterator it = mIds.find(id);
	if ( it == mIds.end() ) {
		// error: id not in id set
		// this would be a good place to throw an exception (when we finaly use them)
		return;
	}

	mIds.erase(it);
}

size_t CollisionGroup::size() const
{
	return mIds.size();
}


}
