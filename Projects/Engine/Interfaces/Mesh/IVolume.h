
#ifndef _Interfaces_Mesh_IVolume_h_
#define _Interfaces_Mesh_IVolume_h_


// Library includes

// Project includes
#include <Math/Tools.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


class IVolume
{
public:
	class Type
	{
	public:
		enum E {
			Undefined = 0,
			AABBox,
			AABBoxNoTranslate,
			Box,
			Cylinder,
			OrientedBoundingBox,
			Sphere
		};
	};

public:
	IVolume(Type::E type = Type::Undefined, const vector3f& size = vector3f())
	: mSize(size),
	  mType(type)
	{
		mCenter = mSize * 0.5f;
	}
	virtual ~IVolume() { }

public:
	virtual void render(bool solid) = 0;

public:
	virtual vector3f center() const {
		return mCenter;
	}

	virtual const vector3f& position() const {
		return mPosition;
	}
	virtual void position(const vector3f& position) {
		mPosition = position;
	}

	virtual float radius() const {
		return magnitude(mSize) * 0.5f;
	}

	virtual vector3f size() {
		return mSize;
	}
	virtual void size(const vector3f& size) {
		mSize = size;
		mCenter = mSize * 0.5f;
	}

	virtual vector3f translation() const {
		return mTranslation;
	}
	virtual void translation(const vector3f& translation) {
		mTranslation = translation;
	}

	virtual Type::E type() const {
		return mType;
	}
	virtual void type(Type::E type) {
		mType = type;
	}

protected:
	vector3f	mCenter;
	vector3f	mPosition;
	vector3f	mSize;
	vector3f	mTranslation;
	Type::E		mType;

private:

};


#endif
