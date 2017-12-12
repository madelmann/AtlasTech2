
#ifndef Arena_Collision_Manager_h
#define Arena_Collision_Manager_h


// Library includes
#include <list>
#include <map>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Common/AManager.h>
#include <Physic/CollisionGroup.h>

// Forward declarations
namespace Scene {
	class SceneGraphObject;
}

// Namespace declarations


namespace Collision {


class Manager : public Common::AManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

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

	void registerSGO(Scene::SceneGraphObject *sgo);
	void unregisterSGO(Scene::SceneGraphObject *sgo);

	const Physics::CollisionGroupsList& getCollisionGroups();

protected:

private:
	typedef std::list<Scene::SceneGraphObject*> SGOList;
	//typedef std::list<Core::AEid> CollisionIdentifierList;
	//typedef std::map<Core::AEid, CollisionIdentifierList> CollisionIdentifiers;

private:
	SGOList::iterator find(Scene::SceneGraphObject *sgo);
	void updateCollisionGroups();

private:
	Physics::CollisionGroupsList	mCollisionGroups;
	SGOList	mRegisteredSGOs;
};


}


#endif
