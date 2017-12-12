
#ifndef _Interfaces_Physics_ICollider_h_
#define _Interfaces_Physics_ICollider_h_


// Library includes

// Project includes
#include <Interfaces/Core/Types.h>

// Forward declarations

// Namespace declarations


namespace Physics {


class ICollider
{
public:
	virtual ~ICollider() { }

public:
	virtual Core::AEid collisionGroup() const = 0;
};


}


#endif
