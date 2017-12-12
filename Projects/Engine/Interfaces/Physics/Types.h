
#ifndef _Interfaces_Physics_Types_h_
#define _Interfaces_Physics_Types_h_


// Library includes

// Project includes
#include <Tools/Tools.h>

// Forward declarations

// Namespace declarations


namespace Physics {


static const float StaticObjectMass = 0.f;


class CollisionType
{
public:
	enum E {
		None = 0,		// Collide with nothing
		All = BIT(0),	// Collide with everything
		Other = BIT(1),	// Collide with everything
	};
};


class Shape
{
public:
	enum E {
		Capsule = 0,
		Cone,
		ConvexInternalAabbCaching,
		Cylinder,
		MinkowskiSum,
		PolyhedralConvex,
		Sphere
	};
};


static short CollideWithNothing = CollisionType::None;
static short CollideWithAll = CollisionType::All | CollisionType::Other;
static short CollideWithOther = CollisionType::Other;


}


#endif
