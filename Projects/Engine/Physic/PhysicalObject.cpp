
// Header
#include "PhysicalObject.h"

// Library includes
#include <btBulletDynamicsCommon.h>

// Project includes
#include "MotionState.h"
#include <Scene\SceneGraphObject.h>

// Namespace declarations


namespace Physics {


PhysicalObject::PhysicalObject(Core::AEid id, btDiscreteDynamicsWorld *world, Shape::E shape, float mass)
: Core::Id(id),
  mCollisionGroup(0),
  mCollisionMask(0),
  mCollisionShape(0),
  mIsReady(false),
  mMass(mass),
  mMotionState(0),
  mRigidBody(0),
  mSceneGraphObject(0),
  mShapeType(shape),
  mWorld(world)
{
}

PhysicalObject::~PhysicalObject()
{
	if ( mWorld ) {
		mWorld->removeRigidBody(mRigidBody);
	}

	if ( mRigidBody ) {
		delete mRigidBody;
	}

	if ( mMotionState ) {
		delete mMotionState;
		mMotionState = 0;
	}
}

btCollisionShape* PhysicalObject::collisionShape() const
{
	return mCollisionShape;
}

void PhysicalObject::collisionShape(btCollisionShape *shape)
{
	mCollisionShape = shape;
}

vector3f PhysicalObject::angularVelocity() const
{
	btVector3 v = mRigidBody->getAngularVelocity();

	return vector3f(v.x(), v.y(), v.z());
}

void PhysicalObject::angularVelocity(const vector3f& v)
{
	mRigidBody->setAngularVelocity(btVector3(v.x, v.y, v.z));
}

btCollisionObject::CollisionFlags PhysicalObject::getCollisionFlags() const
{
	return mCollisionFlags;
}

short PhysicalObject::getCollisionGroup() const
{
	return mCollisionGroup;
}

short PhysicalObject::getCollisionMask() const
{
	return mCollisionMask;
}

float PhysicalObject::getMass() const
{
	return mMass;
}

AEMotionState* PhysicalObject::getMotionState() const
{
	return mMotionState;
}

vector3f PhysicalObject::getPosition() const
{
	return mPosition;
}

btRigidBody* PhysicalObject::getRigidBody() const
{
	return mRigidBody;
}

vector3f PhysicalObject::getRotation() const
{
	return mRotation;
}

SceneGraphObject* PhysicalObject::getSceneGraphObject() const
{
	return mSceneGraphObject;
}

Shape::E PhysicalObject::getShapeType() const
{
	return mShapeType;
}

void PhysicalObject::init(const vector3f& position, const vector3f& rotation)
{
	assert(!mIsReady);

	setPosition(position);
	setRotation(rotation);

	if ( mMotionState ) {
		delete mMotionState;
		mMotionState = 0;
	}

	mMotionState = new AEMotionState(
		btTransform(
			btQuaternion(mRotation.x, mRotation.y, mRotation.z, 1),
			btVector3(mPosition.x, mPosition.y, mPosition.z)
		),
		mSceneGraphObject
	);

	btVector3 fallInertia(0, 0, 0);
	mRigidBody = new btRigidBody(
		btRigidBody::btRigidBodyConstructionInfo(mMass, mMotionState, mCollisionShape, fallInertia)
	);
	mWorld->addRigidBody(mRigidBody, mCollisionGroup, mCollisionMask);

	mIsReady = true;
}

vector3f PhysicalObject::linearVelocity() const
{
	btVector3 l = mRigidBody->getLinearVelocity();

	return vector3f(l.x(), l.y(), l.z());
}

void PhysicalObject::linearVelocity(const vector3f& v)
{
	mRigidBody->setLinearVelocity(btVector3(v.x, v.y, v.z));
}

void PhysicalObject::setCollisionFlags(btCollisionObject::CollisionFlags flags)
{
	mCollisionFlags = flags;
}

void PhysicalObject::setCollisionGroup(short group)
{
	mCollisionGroup = group;
}

void PhysicalObject::setCollisionMask(short mask)
{
	mCollisionMask = mask;
}

void PhysicalObject::setMass(float mass)
{
	mMass = mass;
}

void PhysicalObject::setMotionState(AEMotionState *motion)
{
	mMotionState = motion;
}

void PhysicalObject::setMotionState(const vector3f& position, const vector3f& rotation)
{
	if ( mMotionState ) {
		delete mMotionState;
		mMotionState = 0;
	}

	mMotionState = new AEMotionState(
		btTransform(
			btQuaternion(rotation.x, rotation.y, rotation.z, 1),
			btVector3(position.x, position.y, position.z)
		),
		mSceneGraphObject
	);
}

void PhysicalObject::setPosition(const vector3f& pos)
{
	mPosition = pos;
}

void PhysicalObject::setRigidBody(btRigidBody *body)
{
	mRigidBody = body;
}

void PhysicalObject::setRotation(const vector3f& rotation)
{
	mRotation = rotation;
}

void PhysicalObject::setSceneGraphObject(SceneGraphObject *sgo)
{
	mSceneGraphObject = sgo;
}

void PhysicalObject::update(float /*elapsedTime*/)
{
	//btTransform trans;
	//mMotionState->getWorldTransform(trans);

	//mPosition = vector3f(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	//mRotation = vector3f(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
}


}
