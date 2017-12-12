
#ifndef Arena_Player_Player_h
#define Arena_Player_Player_h


// Library includes

// Project includes
#include <Entity/Entity.h>
#include <Weapon/Weapon.h>

// Forward declarations

// Namespace declarations


class Player : public Entity
{
public:
	Player(Core::AEid id);

public:
	WeaponSpace::Weapon *mWeapon;

protected:

private:

};


#endif
