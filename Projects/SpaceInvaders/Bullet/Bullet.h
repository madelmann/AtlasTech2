
#ifndef _Bullet_Bullet_h_
#define _Bullet_Bullet_h_


// Library includes

// Project includes
#include <Interfaces/Core/Id.h>
#include <Math/Vector3.h>

// Forward declarations
namespace Scene {
	class SceneGraphObject;
}

// Namespace declarations
using Scene::SceneGraphObject;


class Bullet : public Core::Id
{
public:
	Bullet(Core::AEid id);
	virtual ~Bullet();

public:
	void init(const vector3f& pos, const vector3f& rot);
	void update(float elapsedTime);

	float getDamage() const;
	void setDamage(float damage);

	const vector3f& getDirection() const;
	void setDirection(const vector3f& dir);

	float getLifeTime() const;
	void setLifeTime(float lifeTime);

	const vector3f& getPosition() const;
	void setPosition(const vector3f& pos);

	const vector3f& getRotation() const;
	void setRotation(const vector3f& rot);

	SceneGraphObject* getSceneGraphObject() const;
	void setSceneGraphObject(SceneGraphObject *sgo);

	float getSpeed() const;
	void setSpeed(float speed);

protected:

private:
	float				mDamage;
	vector3f			mDirection;
	float				mLifeTime;
	SceneGraphObject	*mSceneGraphObject;
	float				mSpeed;
};


#endif
