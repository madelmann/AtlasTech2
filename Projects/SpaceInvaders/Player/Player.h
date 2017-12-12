
#ifndef _Player_Player_h_
#define _Player_Player_h_


// Library includes

// Project includes
#include <Math\Vector3.h>

// Forward declarations
namespace Core {
	class RenderObject;
}

// Namespace declarations


class Player
{
public:
	Player();

public:
	int getHealth() const;
	void setHealth(int health);

	void setModel(Core::RenderObject *model);

	const vector3f& getPosition() const;
	void setPosition(const vector3f& position);

	const vector3f& getRotation() const;
	void setRotation(const vector3f& rotation);

protected:

private:
	int					mHealth;
	Core::RenderObject	*mModel;
	vector3f			mPosition;
	vector3f			mRotation;
};


#endif
