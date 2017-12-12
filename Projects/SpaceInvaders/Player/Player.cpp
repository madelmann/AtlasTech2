
// Header
#include "Player.h"

// Library includes

// Project includes
#include <Renderer\RenderObject.h>

// Namespace declarations


Player::Player()
: mHealth(-1)
{
}

int Player::getHealth() const
{
	return mHealth;
}

const vector3f& Player::getPosition() const
{
	return mPosition;
}

const vector3f& Player::getRotation() const
{
	return mRotation;
}

void Player::setHealth(int health)
{
	mHealth = health;
}

void Player::setModel(Core::RenderObject *model)
{
	mModel = model;
}

void Player::setPosition(const vector3f& position)
{
	mPosition = position;
	//mModel->setPosition(mPosition);
}

void Player::setRotation(const vector3f& rotation)
{
	mRotation = rotation;
	//mModel->setRotation(rotation);
}
