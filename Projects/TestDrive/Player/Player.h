
#ifndef _Player_Player_h_
#define _Player_Player_h_


// Library includes

// Project includes
#include <Renderer\RenderObject.h>

// Forward declarations
class vector3f;
class Vehicle;

// Namespace declarations


class Player
{
public:
	Player();
	virtual ~Player();

	void checkMovement();

	vector3f cameraOffset() const;
	vector3f view() const;

	const vector3f& getPosition() const;
	void setPosition(const vector3f& pos);

	const vector3f& getRotation() const;
	void setRotation(const vector3f& rot);

	Vehicle* getVehicle() const;
	void setVehicle(Vehicle *vehicle);

protected:

private:
	Vehicle	*mVehicle;
};


#endif
