
#ifndef _Interfaces_Camera_IViewer_h_
#define _Interfaces_Camera_IViewer_h_


// Library includes

// Project includes
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace CameraSpace {


class IViewer
{
public:
	virtual ~IViewer() { }

	virtual void look() = 0;
	virtual void update(float elapsedTime) = 0;

	virtual const float& fov() const = 0;
	virtual void fov(const float& fov) = 0;

	virtual const vector3f& position() const = 0;
	virtual void position(const vector3f& pos) = 0;

	virtual const vector3f& rotation() const = 0;
	virtual void rotation(const vector3f& rotation) = 0;

	virtual const vector3f& view() const = 0;
	virtual void view(const vector3f& view) = 0;

	virtual const vector3f& upVector() const = 0;
	virtual void upVector(const vector3f& up) = 0;
};


}


#endif
