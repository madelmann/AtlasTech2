
#ifndef Camera_MovieCamera_Action_h
#define Camera_MovieCamera_Action_h


// Library includes

// Project includes
#include <Math/Vector3.h>

// Forward declarations

// Namespace declaration


namespace CameraSpace {


class Action
{
public:
	class Type
	{
	public:
		enum E {
			Default = 0,
			Move,
			Rotate
		};
	};

public:
	Action(Type::E type)
	: mDuration(0.f),
	  mType(type)
	{ }

	float duration() { return mDuration; }
	void duration(float duration) { mDuration = duration; }

	Type::E type() { return mType; }

private:
	float	mDuration;
	Type::E	mType;
};


class MoveAction : public Action
{
public:
	MoveAction(const vector3f& position)
	: Action(Type::Move)
	{
		mPosition = position;
	}

	vector3f position() const {
		return mPosition;
	}

private:
	vector3f	mPosition;
};


class RotateAction : public Action
{
public:
	RotateAction(const vector3f& rotation)
	: Action(Type::Rotate)
	{
		mRotation = rotation;
	}

	vector3f rotation() const {
		return mRotation;
	}

private:
	vector3f	mRotation;
};


}


#endif
