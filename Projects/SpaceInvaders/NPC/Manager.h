
#ifndef NPC_Manager_h
#define NPC_Manager_h


// Library includes

// Project includes
#include "NPC.h"
#include "Settings.h"
#include <Common/Logger.h>
#include <Interfaces/Common/AManager.h>

// Forward declarations
namespace Collision {
	class Manager;
}
class IMediaPathManager;
namespace Mesh {
	class IObjectManager;
}
namespace Scene {
	class IManager;
}

// Namespace declarations


namespace NPCSpace {


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

	// Connectors
	// {
	void connectCollisionManager(Collision::Manager *m);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectObjectManager(Mesh::IObjectManager *m);
	void connectSceneManager(Scene::IManager *m);

	//void connectIdGenerator(Core::IdGenerator *g);
	//void connectPhysicalManager(Physics::IManager *m);
	//void connectTextureManager(TextureSpace::IManager *m);
	// }

	bool hit(Scene::SceneGraphObject *npc);
	bool isThereAnEnemyStillAlive() const;

protected:

private:
	NPCList::iterator find(Scene::SceneGraphObject *npc);
	void killNPC(NPC *npc);
	void loadConfig();

private:
	NPCList		mNPCs;
	Settings	mSettings;

	Collision::Manager		*mCollisionManager;
	IMediaPathManager		*mMediaPathManager;
	Mesh::IObjectManager	*mObjectManager;
	Scene::IManager			*mSceneManager;

	//Core::IdGenerator		*mIdGenerator;
	//Physics::IManager		*mPhysicsManager;
	//TextureSpace::IManager	*mTextureManager;
};


}


#endif
