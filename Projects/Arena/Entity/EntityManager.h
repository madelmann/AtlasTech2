
#ifndef Arena_Entity_EntityManager_h
#define Arena_Entity_EntityManager_h


// Library includes
#include <list>
#include <map>

// Project includes
#include "IEntityManager.h"

// Forward declarations
class Entity;
namespace Mesh {
	class IObjectManager;
}
namespace Scene {
	class IManager;
}


class EntityManager : public IEntityManager
{
public:
	typedef std::map<Core::AEid, Entity*> EntityMap;

public:
	EntityManager();
	~EntityManager();

public:
	void connectObjectManager(Mesh::IObjectManager *m);
	void connectSceneManager(Scene::IManager *m);

public:	// IEntityManager implementation
	void add(Core::AEid id);
	Entity* getEntity(Core::AEid id) const;
	void remove(Core::AEid id);
	void updatePosition(Core::AEid id, const vector3f& value);
	void updateRotation(Core::AEid id, const vector3f& value);

protected:

private:
	EntityMap mEntities;

	Mesh::IObjectManager *mObjectManager;
	Scene::IManager *mSceneManager;
};


#endif
