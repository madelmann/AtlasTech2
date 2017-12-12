
#ifndef Types_h
#define Types_h


// Library includes

// Project includes
#include <Tools/Tools.h>

// Forward declarations

// Namespace declarations


static const Core::AEid PlayerShip = 10;
static const Core::AEid PlayerBullet = 11;
static const Core::AEid EnemyShip = 20;
static const Core::AEid EnemyBullet = 21;
static const Core::AEid PowerUp = 30;


class CollisionType
{
public:
	enum E {
		NOTHING = 0,		// Collide with nothing
		SHIP = BIT(0),		// Collide with ships
		WALL = BIT(1),		// Collide with walls
		POWERUP = BIT(2),	// Collide with powerups
		PLAYER = BIT(3),	// Collide with player
		BULLET = BIT(4)		// Collide with bullet
	};
};


static short bulletCollidesWith = CollisionType::SHIP;
static short playerCollidesWith = CollisionType::PLAYER;// | CollisionType::POWERUP;
static short powerupCollidesWith = /*CollisionType::PLAYER | CollisionType::POWERUP |*/ CollisionType::SHIP | CollisionType::WALL;
static short shipCollidesWith = CollisionType::BULLET | CollisionType::PLAYER | /*CollisionType::POWERUP |*/ CollisionType::SHIP | CollisionType::WALL;
static short wallCollidesWith = CollisionType::NOTHING;// | CollisionType::WALL;


#endif
