
// Header
#include "Weapon.h"

// Project includes
#include <Driver/GLWrapper.h>
#include <Renderer/RenderObject.h>


namespace WeaponSpace {


Weapon::Weapon(Core::AEid id, const std::string& name)
: Core::Object(id, name),
  mModel(0),
  mTime(0.f)
{
}

Weapon::~Weapon()
{
}

void Weapon::render()
{
	if ( !mModel ) {
		return;
	}

	static float aplitude = 0.05f;
	static vector3f move;

	{	// Lissajous
		move.x = aplitude * sin(mTime);
		move.y = aplitude * sin(mTime) * 0.5f;
	}

	aePushMatrix();
		aeDepthMask(false);
		aeDisable(GL_DEPTH_TEST);

		aeTranslate(mPosition + move);
		aeRotate(mRotation);

		mModel->render();
	aePopMatrix();
}

void Weapon::setModel(Core::RenderObject *m)
{
	mModel = m;
}

void Weapon::setPosition(const vector3f& pos)
{
	mPosition = pos;
}

void Weapon::setRotation(const vector3f& rot)
{
	mRotation = rot;
}

void Weapon::update(float elapsedTime)
{
	mTime += elapsedTime;
}


}
