
#ifndef _Physic_PhysicalObject_h_
#define _Physic_PhysicalObject_h_


// Library includes
#include <btBulletDynamicsCommon.h>

// Project includes
#include "ACollider.h"
#include <Interfaces\Core\Id.h>
#include <Interfaces\Physics\Types.h>
#include <Math\Vector3.h>

// Forward declarations
class btCollisionShape;
struct btDefaultMotionState;
class btDiscreteDynamicsWorld;
class btRigidBody;

namespace Scene {
	class SceneGraphObject;
}

// Namespace declarations
using Scene::SceneGraphObject;


namespace Physics {

// Forward declarations
class AEMotionState;

class PhysicalObject : public Core::Id,
					   public ACollider
{
public:
	PhysicalObject(Core::AEid id, btDiscreteDynamicsWorld *world, Shape::E shape, float mass);
	virtual ~PhysicalObject();

public:
	void init(const vector3f& position, const vector3f& rotation);
	void update(float elapsedTime = 0.f);

	vector3f angularVelocity() const;
	void angularVelocity(const vector3f& v);

	btCollisionShape* collisionShape() const;
	void collisionShape(btCollisionShape *shape);

	vector3f linearVelocity() const;
	void linearVelocity(const vector3f& v);


	btCollisionObject::CollisionFlags getCollisionFlags() const;
	void setCollisionFlags(btCollisionObject::CollisionFlags flags);

	short getCollisionGroup() const;
	void setCollisionGroup(short group);

	short getCollisionMask() const;
	void setCollisionMask(short mask);

	float getMass() const;
	void setMass(float mass);

	AEMotionState* getMotionState() const;
	void setMotionState(AEMotionState *motion);
	void setMotionState(const vector3f& position, const vector3f& rotation);

	vector3f getPosition() const;
	void setPosition(const vector3f& pos);

	btRigidBody* getRigidBody() const;
	void setRigidBody(btRigidBody *body);

	vector3f getRotation() const;
	void setRotation(const vector3f& rotation);

	SceneGraphObject* getSceneGraphObject() const;
	void setSceneGraphObject(SceneGraphObject *sgo);

	Shape::E getShapeType() const;

	const vector3f& size() const { return mSize; }
	void size(const vector3f& size) { mSize = size; }

protected:

private:
	short		mCollisionGroup;
	short		mCollisionMask;
	bool		mIsReady;
	float		mMass;
	vector3f	mPosition;
	vector3f	mRotation;
	Shape::E	mShapeType;
	vector3f	mSize;

	AEMotionState		*mMotionState;
	SceneGraphObject	*mSceneGraphObject;

	btCollisionObject::CollisionFlags	mCollisionFlags;
	btCollisionShape					*mCollisionShape;
	btRigidBody							*mRigidBody;
	btDiscreteDynamicsWorld				*mWorld;
};


}


#endif
