
#ifndef Arena_Weapon_Weapon_h
#define Arena_Weapon_Weapon_h


// Project includes
#include <Interfaces/Core/Object.h>
#include <Math/Vector3.h>

// Forward declarations
namespace Core {
	class RenderObject;
}


namespace WeaponSpace {


class Weapon : public Core::Object
{
public:
	Weapon(Core::AEid id, const std::string& name);
	~Weapon();

public:	// Setup
	void setModel(Core::RenderObject *m);
	void setPosition(const vector3f& pos);
	void setRotation(const vector3f& rot);

public:
	void render();
	void update(float elapsedTime);

protected:

private:
	Core::RenderObject *mModel;
	vector3f mPosition;
	vector3f mRotation;
	float mTime;
};


}


#endif
