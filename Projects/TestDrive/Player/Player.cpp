
// Header
#include "Player.h"

// Library includes

// Project includes
#include <Camera\Viewer.h>
#include <Driver\Keyboard.h>
#include <Math\Vector3.h>
#include <Renderer\RenderObject.h>
#include <Vehicle\Vehicle.h>

// Namespace declarations


Player::Player()
: mVehicle(0)
{
}

Player::~Player()
{
}

vector3f Player::cameraOffset() const
{
	return mVehicle->cameraOffset();
}

void Player::checkMovement()
{
	if ( !mVehicle ) {
		return;
	}

	if ( isKeyDown(VK_UP) ) {
		mVehicle->accelerate(Vehicle::PushGas);
	}
	else if ( isKeyDown(VK_DOWN) ) {
		mVehicle->accelerate(Vehicle::PushBreak);
	}
	else {
		mVehicle->accelerate(Vehicle::None);
	}

	if ( isKeyDown(VK_LEFT) ) {
		mVehicle->steer(Vehicle::Left);
	}
	else if ( isKeyDown(VK_RIGHT) ) {
		mVehicle->steer(Vehicle::Right);
	}
	else {
		mVehicle->steer(Vehicle::Straight);
	}
}

const vector3f& Player::getPosition() const
{
	return mVehicle->getPosition();
}

const vector3f& Player::getRotation() const
{
	return mVehicle->getRotation();
}

Vehicle* Player::getVehicle() const
{
	return mVehicle;
}

void Player::setPosition(const vector3f& pos)
{
	mVehicle->setPosition(pos);
}

void Player::setRotation(const vector3f& rot)
{
	mVehicle->setRotation(rot);
}

void Player::setVehicle(Vehicle *vehicle)
{
	mVehicle = vehicle;
}

vector3f Player::view() const
{
	return mVehicle->view();
}
