
#ifndef _Vehicle_Vehicle_h_
#define _Vehicle_Vehicle_h_


// Library includes
#include <string>

// Project includes
#include <Math/Vector3.h>

// Forward declarations
namespace Scene {
	class SceneGraphObject;
}
namespace Terrain {
	class IManager;
}

// Namespace declarations


class Vehicle
{
public:
	typedef enum AccelerationType_e {
		None = 0,
		PushBreak,
		PushGas
	};

	typedef enum SteeringType_e {
		Left = 0,
		Right,
		Straight
	};

public:
	Vehicle();
	virtual ~Vehicle() { }

public:
	void connectTerrainManager(Terrain::IManager *m);

public:
	const vector3f& cameraOffset() const;
	void cameraOffset(const vector3f& offset);

	Scene::SceneGraphObject* sceneGraphObject();
	void sceneGraphObject(Scene::SceneGraphObject *sgo);

	const std::string& getName() const;
	void setName(const std::string& name);

	const vector3f& getPosition() const;
	void setPosition(const vector3f &pos);

	const vector3f& getRotation() const;
	void setRotation(const vector3f& rot);

	vector3f getPositionFront() const;
	vector3f getPositionFrontLeft() const;
	vector3f getPositionFrontRight() const;
	vector3f getPositionRear() const;
	vector3f getPositionRearLeft() const;
	vector3f getPositionRearRight() const;

	const vector3f& view() const;
	void view(const vector3f& view);

	void accelerate(AccelerationType_e acc);
	void steer(SteeringType_e steer);

	void update(float elapsedTime);

	void setAirDecceleration(float dec);
	void setAxle(const vector3f& axle);
	void setBreakDecceleration(float dec);
	void setCurveAngle(float angle);
	void setMass(float mass);
	void setMaxAcceleration(float max);
	void setMaxCurveAngle(float angle);
	void setMaxSpeed(float max);

protected:

private:
	AccelerationType_e	mAccelerationType;
	float				mAirDecceleration;
	vector3f			mAxle;
	float				mBreakDecceleration;
	vector3f			mCameraOffset;
	float				mCurrentAcceleration;
	float				mCurrentCurveAngle;
	float				mCurrentSpeed;
	SteeringType_e		mCurrentSteering;
	float				mCurveAnglePerSecond;
	float				mMass;
	float				mMaxAcceleration;
	float				mMaxCurveAngle;
	float				mMaxSpeed;
	vector3f			mMiddleNormal;
	std::string			mName;
	vector3f			mPosition;
	vector3f			mRotation;
	vector3f			mView;

	Scene::SceneGraphObject	*mSceneGraphObject;
	Terrain::IManager *mTerrainManager;
};


#endif
