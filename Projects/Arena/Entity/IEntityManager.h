
#ifndef Arena_Entity_IEntityManager_h
#define Arena_Entity_IEntityManager_h


// Library includes

// Project includes
#include <Interfaces/Core/Types.h>
#include <Math/Vector3.h>

// Forward declarations
class Entity;

// Namespace declarations


class IEntityManager
{
public:
	virtual ~IEntityManager() { }

	virtual void add(Core::AEid id) = 0;
	virtual Entity* getEntity(Core::AEid id) const = 0;
	virtual void remove(Core::AEid id) = 0;
	virtual void updatePosition(Core::AEid id, const vector3f& value) = 0;
	virtual void updateRotation(Core::AEid id, const vector3f& value) = 0;
};


#endif
