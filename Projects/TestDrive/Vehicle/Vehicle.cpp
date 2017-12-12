
// Header
#include "Vehicle.h"

// Library includes
#include <cmath>

// Project includes
#include <Interfaces/Terrain/IManager.h>
#include <Math/Tools.h>
#include <Scene/SceneGraphObject.h>

// Namespace declarations


Vehicle::Vehicle()
: mAccelerationType(None),
  mAirDecceleration(0.f),
  mBreakDecceleration(0.f),
  mCurrentAcceleration(0.f),
  mCurrentCurveAngle(0.f),
  mCurrentSpeed(0.f),
  mCurrentSteering(Straight),
  mCurveAnglePerSecond(0.f),
  mMass(0.f),
  mMaxAcceleration(0.f),
  mMaxCurveAngle(0.f),
  mMaxSpeed(0.f),
  mSceneGraphObject(0),
  mTerrainManager(0)
{
}

void Vehicle::accelerate(AccelerationType_e acc)
{
	mAccelerationType = acc;

	switch ( mAccelerationType ) {
		case None:
			mCurrentAcceleration = -mAirDecceleration;							// This values is negative
			break;
		case PushBreak:
			mCurrentAcceleration = -(mAirDecceleration + mBreakDecceleration);	// This values is negative
			break;
		case PushGas:
			mCurrentAcceleration = mMaxAcceleration;							// This values is positive
			break;
	}
}

const vector3f& Vehicle::cameraOffset() const
{
	return mCameraOffset;
}

void Vehicle::cameraOffset(const vector3f& offset)
{
	mCameraOffset = offset;
}

void Vehicle::connectTerrainManager(Terrain::IManager *m)
{
	mTerrainManager = m;
}

const std::string& Vehicle::getName() const
{
	return mName;
}

const vector3f& Vehicle::getPosition() const
{
	return mPosition;
}

vector3f Vehicle::getPositionFront() const
{
	vector3f pos = getPosition();

	pos.x += cos(mRotation.y * PI / 180.f);
	pos.z += sin(mRotation.y * PI / 180.f) * -mAxle.z;

	return pos;
}

vector3f Vehicle::getPositionFrontLeft() const
{
	vector3f pos = getPosition();

	pos.x += cos(mRotation.y * PI / 180.f) * -mAxle.x;
	pos.z += sin(mRotation.y * PI / 180.f) * -mAxle.z;

	return pos;
}

vector3f Vehicle::getPositionFrontRight() const
{
	vector3f pos = getPosition();

	pos.x += cos(mRotation.y * PI / 180.f) *  mAxle.x;
	pos.z += sin(mRotation.y * PI / 180.f) * -mAxle.z;

	return pos;
}

vector3f Vehicle::getPositionRear() const
{
	vector3f pos = getPosition();

	pos.x += cos(mRotation.y * PI / 180.f);
	pos.z += sin(mRotation.y * PI / 180.f) * mAxle.z;

	return pos;
}

vector3f Vehicle::getPositionRearLeft () const
{
	vector3f pos = getPosition();

	pos.x += cos(mRotation.y * PI / 180.f) * -mAxle.x;
	pos.z += sin(mRotation.y * PI / 180.f) *  mAxle.z;

	return pos;
}

vector3f Vehicle::getPositionRearRight() const
{
	vector3f pos = getPosition();

	pos.x += cos(mRotation.y * PI / 180.f) * mAxle.x;
	pos.z += sin(mRotation.y * PI / 180.f) * mAxle.z;

	return pos;
}

const vector3f& Vehicle::getRotation() const
{
	return mRotation;
}

Scene::SceneGraphObject* Vehicle::sceneGraphObject()
{
	return mSceneGraphObject;
}

void Vehicle::sceneGraphObject(Scene::SceneGraphObject *sgo)
{
	mSceneGraphObject = sgo;
}

void Vehicle::setAirDecceleration(float dec)
{
	mAirDecceleration = dec;
}

void Vehicle::setAxle(const vector3f& axle)
{
	mAxle = axle;
}

void Vehicle::setBreakDecceleration(float dec)
{
	mBreakDecceleration = dec;
}

void Vehicle::setCurveAngle(float angle)
{
	mCurveAnglePerSecond = angle;
}

void Vehicle::setMass(float mass)
{
	mMass = mass;
}

void Vehicle::setMaxAcceleration(float max)
{
	mMaxAcceleration = max;
}

void Vehicle::setMaxCurveAngle(float angle)
{
	mMaxCurveAngle = angle;
}

void Vehicle::setMaxSpeed(float max)
{
	mMaxSpeed = max;
}

void Vehicle::setName(const std::string& name)
{
	mName = name;
}

void Vehicle::setPosition(const vector3f &pos)
{
	mPosition = pos;

	mSceneGraphObject->setPosition(pos);
}

void Vehicle::setRotation(const vector3f& rot)
{
	mRotation = rot;

	mSceneGraphObject->setRotation(mRotation);
}

void Vehicle::steer(SteeringType_e steer)
{
	mCurrentSteering = steer;
}

void Vehicle::update(float elapsedTime)
{
	// Steering
	{
		switch ( mCurrentSteering ) {
			case Left:
				mCurrentCurveAngle += mCurveAnglePerSecond * elapsedTime;
				break;
			case Right:
				mCurrentCurveAngle -= mCurveAnglePerSecond * elapsedTime;
				break;
			case Straight:
				if ( mCurrentCurveAngle < -mCurveAnglePerSecond * elapsedTime ) {
					mCurrentCurveAngle += mCurveAnglePerSecond * elapsedTime;
				}
				else if (mCurrentCurveAngle > mCurveAnglePerSecond * elapsedTime ) {
					mCurrentCurveAngle -= mCurveAnglePerSecond * elapsedTime;
				}
				else {
					mCurrentCurveAngle = 0.f;
				}
				break;
		}

		if ( mCurrentCurveAngle < -mMaxCurveAngle * elapsedTime  ) {
			mCurrentCurveAngle = -mMaxCurveAngle * elapsedTime;
		}
		if ( mCurrentCurveAngle > mMaxCurveAngle * elapsedTime ) {
			mCurrentCurveAngle = mMaxCurveAngle * elapsedTime;
		}
	}

	// Acceleration & speed
	{
		mCurrentSpeed += mCurrentAcceleration * elapsedTime;

		if ( mCurrentSpeed <= 0.f ) {
			mCurrentSpeed = 0.f;
		}
		if ( mCurrentSpeed > mMaxSpeed * elapsedTime ) {
			mCurrentSpeed = mMaxSpeed * elapsedTime;
		}
	}


	// Only rotate if we are driving
	if ( mCurrentSpeed != 0.f ) {
		mRotation.y += mCurrentCurveAngle;
	}

	if ( mRotation.y < 0.f ) {
		mRotation.y += 360.f;
	}
	else if ( mRotation.y >= 360.f ) {
		mRotation.y -= 360.f;
	}


	vector3f view;
	view.x = sin(mRotation.y * PI / 180.f);
	//view.y = sin(sqrt(mRotation.x * mRotation.x + mRotation.z * mRotation.z) * PI / 180.f);
	view.z = cos(mRotation.y * PI / 180.f);

	mPosition.x += view.x * mCurrentSpeed;
	mPosition.z += view.z * mCurrentSpeed;
/*
	mPosition = mTerrainManager->heightAt(mPosition);

	vector3f fl = mTerrainManager->heightAt((*it)->getPositionFrontLeft());
	vector3f fr = mTerrainManager->heightAt((*it)->getPositionFrontRight());
	vector3f rl = mTerrainManager->heightAt((*it)->getPositionRearLeft());
	vector3f rr = mTerrainManager->heightAt((*it)->getPositionRearRight());

	vector3f f = fl - fr;
	vector3f r = rl - rr;

	vector3f c = cross(f, r);


	vector3f mRotation = (*it)->getRotation();
	mRotation.x = acos(sqrt(c.y * c.y + c.z * c.z) / PI * 180.f);
	mRotation.y = acos(sqrt(c.x * c.x + c.z * c.z) / PI * 180.f);
	mRotation.z = acos(sqrt(c.x * c.x + c.y * c.y) / PI * 180.f);
	(*it)->setRotation(mRotation);
*/

	mView = mPosition + view;
}

const vector3f& Vehicle::view() const
{
	return mView;
}

void Vehicle::view(const vector3f& view)
{
	mView = view;

	//mRotation.x = cos(view.x) / PI * 180.f;
	//mRotation.z = sin(view.z) / PI * 180.f;
}
