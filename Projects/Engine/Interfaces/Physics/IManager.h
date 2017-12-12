
#ifndef _Interfaces_Physics_IManager_h_
#define _Interfaces_Physics_IManager_h_


// Library includes

// Project includes
#include <Interfaces/Common/AManager.h>
#include <Interfaces/Core/Id.h>
#include <Interfaces/Physics/Types.h>
#include <Math/Vector3.h>

// Forward declarations
class btCollisionShape;
class btMotionState;
class btRigidBody;

// Namespace declarations


namespace Physics {

// Forward declarations
class AEMotionState;
class PhysicalObject;

class IManager : public Common::AManager
{
public:
	virtual ~IManager() { }

public:
	virtual PhysicalObject* create(Shape::E shape, float mass) = 0;
	virtual PhysicalObject* create(float mass, const vector3f& size, btCollisionShape *shape) = 0;
	virtual PhysicalObject* createBox(float mass, const vector3f& size) = 0;
	virtual PhysicalObject* createSphere(float mass, float radius) = 0;
	virtual bool remove(PhysicalObject *po) = 0;

	virtual PhysicalObject* findPhysicalObjectById(Core::AEid id) const = 0;

	// Debug only
	// {
	virtual void shootBox(const vector3f& position, const vector3f& view) = 0;
	// }
};


}


#endif
