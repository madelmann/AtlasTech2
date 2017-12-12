
// Header
#include "ACollider.h"

// Library includes

// Project includes

// Namespace declarations


namespace Physics {


ACollider::ACollider()
: mGroupId(Core::invalidID)
{
}

Core::AEid ACollider::collisionGroup() const
{
	return mGroupId;
}

void ACollider::collisionGroup(Core::AEid groupId)
{
	mGroupId = groupId;
}


}
