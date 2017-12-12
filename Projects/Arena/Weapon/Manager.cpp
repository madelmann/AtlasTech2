
// Header
#include "Manager.h"

// Library includes

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Tools/Files.h>
#include "Weapon.h"

// Namespace declarations


namespace WeaponSpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "WeaponManager"),
  mObjectManager(0)
{
}

Manager::~Manager()
{
	for ( Weapons::iterator it = mWeapons.begin(); it != mWeapons.end(); ++it ) {
		delete (*it);
	}
	mWeapons.clear();
}

void Manager::connectObjectManager(Mesh::IObjectManager *m)
{
	mObjectManager = m;
}

Weapon* Manager::create(const std::string& name)
{
	if ( name.empty() ) {
		return 0;
	}

	std::string filename = "base/weapons/" + Tools::Files::RemoveFileExt(name) + ".conf";

	Core::Configuration::Configurator config(this, filename);
	if ( !config.success() ) {
		return 0;
	}

	Weapon *weapon = new Weapon(Core::invalidID, config.getValue("name").toString());
	weapon->setModel(mObjectManager->create(config.getValue("model").toString()));
	weapon->setPosition(config.getValue("position").toVector3f());
	weapon->setRotation(config.getValue("rotation").toVector3f());

	mWeapons.push_back(weapon);
	return weapon;
}


}
