
#ifndef _Physic_ACollider_h_
#define _Physic_ACollider_h_


// Library includes

// Project includes
#include <Interfaces/Physics/ICollider.h>

// Forward declarations

// Namespace declarations


namespace Physics {


class ACollider : public ICollider
{
public:
	virtual ~ACollider() { }

public:
	Core::AEid collisionGroup() const;
	void collisionGroup(Core::AEid groupId);

protected:
	ACollider();

private:
	Core::AEid	mGroupId;
};


}


#endif
