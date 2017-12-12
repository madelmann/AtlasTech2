
#ifndef _Input_Mouse_Mouse_h_
#define _Input_Mouse_Mouse_h_


// Library includes

// Project includes
#include <Interfaces/Input/IMouse.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Input {


class Mouse : public IMouse
{
public:
	Mouse();
	~Mouse();

public:
	// IMousePositioner implementation
	// {
	void setPosition2D(const vector2l& pos);
	void setPosition3D(const vector3f& pos);
	// }

	// IMouse implementation
	// {
	const vector2l& getPosition2D() const;
	const vector3f& getPosition3D() const;
	// }

protected:

private:
	vector2l	mPosition2D;
	vector3f	mPosition3D;
};


}


#endif
