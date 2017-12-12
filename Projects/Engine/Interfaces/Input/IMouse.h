
#ifndef _Interfaces_Input_IMouse_h_
#define _Interfaces_Input_IMouse_h_


// Library includes

// Project includes
#include <Math/Vector2.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarationd


namespace Input {


class IMousePositioner
{
public:
	virtual ~IMousePositioner() { }

public:
	virtual void setPosition2D(const vector2l& pos) = 0;
	virtual void setPosition3D(const vector3f& pos) = 0;
};


class IMouse : private IMousePositioner
{
public:
	virtual ~IMouse() { }

public:
	virtual const vector2l& getPosition2D() const = 0;
	virtual const vector3f& getPosition3D() const = 0;
};


}


#endif
