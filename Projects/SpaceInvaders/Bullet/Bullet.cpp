
// Header
#include "Bullet.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Mesh/Volume/AABBox.h>
#include <Renderer/RenderObject.h>
#include <Scene/SceneGraphObject.h>
#include <Tools/Tools.h>

// Namespace declarations


Bullet::Bullet(Core::AEid id)
: Core::Id(id),
  mDamage(1.f),
  mLifeTime(0.f),
  mSceneGraphObject(0),
  mSpeed(0.f)
{
}

Bullet::~Bullet()
{
}

float Bullet::getDamage() const
{
	return mDamage;
}

const vector3f& Bullet::getDirection() const
{
	return mDirection;
}

float Bullet::getLifeTime() const
{
	return mLifeTime;
}

const vector3f& Bullet::getPosition() const
{
	return mSceneGraphObject->getPosition();
}

const vector3f& Bullet::getRotation() const
{
	return mSceneGraphObject->getRotation();
}

SceneGraphObject* Bullet::getSceneGraphObject() const
{
	return mSceneGraphObject;
}

float Bullet::getSpeed() const
{
	return mSpeed;
}

void Bullet::init(const vector3f& pos, const vector3f& rot)
{
	mSceneGraphObject->init(pos, rot);
}

void Bullet::setDamage(float damage)
{
	mDamage = damage;
}

void Bullet::setDirection(const vector3f& dir)
{
	mDirection = dir;
}

void Bullet::setLifeTime(float lifeTime)
{
	mLifeTime = lifeTime;
}

void Bullet::setPosition(const vector3f& pos)
{
	if ( mSceneGraphObject ) {
		mSceneGraphObject->setPosition(pos);
	}
}

void Bullet::setRotation(const vector3f& rot)
{
	if ( mSceneGraphObject ) {
		mSceneGraphObject->setRotation(rot);
	}
}

void Bullet::setSceneGraphObject(SceneGraphObject *sgo)
{
	mSceneGraphObject = sgo;
}

void Bullet::setSpeed(float speed)
{
	mSpeed = speed;
}

void Bullet::update(float elapsedTime)
{
	setPosition(mDirection * mSpeed * elapsedTime + getPosition());

	mLifeTime -= elapsedTime;
}
