
#ifndef Bullets_BulletManager_h
#define Bullets_BulletManager_h


// Library includes
#include <list>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Common/AManager.h>
#include <Math/Vector3.h>

// Forward declarations
class Bullet;
namespace Collision {
	class Manager;
}
namespace Core {
	class IdGenerator;
}
class IVolume;
namespace Mesh {
	class IObjectManager;
}
namespace Scene {
	class IManager;
	class SceneGraphObject;
}
namespace TextureSpace {
	class IManager;
}

// Namespace declarations


class BulletManager : public Common::AManager,
					  private Common::Logger
{
public:
	typedef std::list<Bullet*>	BulletList;

public:
	BulletManager(const Common::Logger *p);
	~BulletManager();

public:
	// Common::AManager implementation
	// {
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);
	// }

	// Connectors
	// {
	void connectCollisionManager(Collision::Manager *m);
	void connectIdGenerator(Core::IdGenerator *g);
	void connectObjectManager(Mesh::IObjectManager *m);
	void connectSceneManager(Scene::IManager *m);
	void connectTextureManager(TextureSpace::IManager *m);
	// }

	void add(const vector3f& pos, const vector3f& rot);
	void removeReference(Scene::SceneGraphObject *sgo);

protected:

private:
	BulletList	mBullets;

	Collision::Manager		*mCollisionManager;
	Core::IdGenerator		*mIdGenerator;
	Mesh::IObjectManager	*mObjectManager;
	Scene::IManager			*mSceneManager;
	TextureSpace::IManager	*mTextureManager;
};


#endif
