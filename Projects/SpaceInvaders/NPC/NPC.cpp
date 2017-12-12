
// Header
#include "NPC.h"

// Library includes

// Project includes
#include <Scene\SceneGraphobject.h>

// Namespace declarations


namespace NPCSpace {


NPC::NPC()
: mHealth(0.f),
  mSceneGraphObject(0)
{
}

NPC::~NPC()
{
}

Scene::SceneGraphObject* NPC::getSceneGraphObject() const
{
	return mSceneGraphObject;
}

float NPC::health() const
{
	return mHealth;
}

void NPC::health(float health)
{
	mHealth = health;
}

void NPC::init(const vector3f& pos, const vector3f& rot)
{
	if ( mSceneGraphObject ) {
		mSceneGraphObject->init(pos, rot);
	}
}

vector3f NPC::position() const
{
	if ( mSceneGraphObject ) {
		return mSceneGraphObject->getPosition();
	}

	return vector3f();
}

void NPC::position(const vector3f& position)
{
	if ( mSceneGraphObject ) {
		mSceneGraphObject->setPosition(position);
	}
}

vector3f NPC::rotation() const
{
	if ( mSceneGraphObject ) {
		return mSceneGraphObject->getRotation();
	}

	return vector3f();
}

void NPC::rotation(const vector3f& rotation)
{
	if ( mSceneGraphObject ) {
		mSceneGraphObject->setRotation(rotation);
	}
}

void NPC::setSceneGraphObject(Scene::SceneGraphObject *sgo)
{
	mSceneGraphObject = sgo;
}


}
