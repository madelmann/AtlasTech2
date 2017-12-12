
#ifndef Arena_Weapon_Manager_h
#define Arena_Weapon_Manager_h


// Library includes
#include <list>
#include <string>

// Project includes
#include <Common/Logger.h>

// Forward declarations
namespace Mesh {
	class IObjectManager;
}

// Namespace declarations


namespace WeaponSpace {

// Forward declarations
class Weapon;

class Manager : private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

public:
	void connectObjectManager(Mesh::IObjectManager *m);

public:
	Weapon* create(const std::string& name);

protected:

private:
	typedef std::list<Weapon*> Weapons;

private:
	Weapons mWeapons;

	Mesh::IObjectManager *mObjectManager;
};


}


#endif
