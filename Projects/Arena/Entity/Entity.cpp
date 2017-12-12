
// Header
#include "Entity.h"

// Library includes

// Project includes

// Namespace declartions


Entity::Entity(Core::AEid id)
: Core::Id(id),
  mMesh(0)
{
}

Entity::~Entity()
{
}

vector3f Entity::direction() const
{
	return vector3f();
}

void Entity::direction(const vector3f& value)
{
	(void)value;
}

vector3f Entity::position() const
{
	return mMesh->getPosition();
}

void Entity::position(const vector3f& value)
{
	mMesh->setPosition(value);
}

vector3f Entity::rotation() const
{
	return mMesh->getRotation();
}

void Entity::rotation(const vector3f& value)
{
	mMesh->setRotation(value);
}
