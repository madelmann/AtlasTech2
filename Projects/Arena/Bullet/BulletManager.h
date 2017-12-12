
#ifndef Arena_Bullet_BulletManager_h
#define Arena_Bullet_BulletManager_h


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
	// }

	void add(const std::string& name, const vector3f& pos, const vector3f& rot);
	Bullet* create(const std::string& name);
	Bullet* find(const std::string& name) const;
	void removeReference(Scene::SceneGraphObject *sgo);

protected:

private:
	BulletList	mBulletRepository;
	BulletList	mBullets;

	Collision::Manager *mCollisionManager;
	Core::IdGenerator *mIdGenerator;
	Mesh::IObjectManager *mObjectManager;
	Scene::IManager *mSceneManager;
};


#endif
