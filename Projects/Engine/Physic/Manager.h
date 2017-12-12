
#ifndef _Physic_Manager_h_
#define _Physic_Manager_h_


// Library includes
#include <list>
#include <map>

//#define USE_BULLET
#ifdef USE_BULLET
#include <btBulletDynamicsCommon.h>
#endif

// Project includes
#include "Settings.h"
#include <Common/Logger.h>
#include <Interfaces/Core/Id.h>
#include <Interfaces/Physics/IManager.h>

// Forward declarations
namespace Console {
	class IConsoleInterface;
}
namespace Core {
	class IdGenerator;
}
class IMediaPathManager;
class IVolume;
namespace Mesh {
	class IObjectManager;
}
namespace Scene {
	class IManager;
}

// Namespace declarations
using Console::IConsoleInterface;


namespace Physics {

// Forward declarations
class PhysicalObject;
class World;

class Movement
{
public:
	Movement()
	: mInitialMovement(true)
	{ }

	const vector3f& t0() const {
		return mT0;
	}
	const vector3f& t1() const {
		return mT1;
	}

	void updatePosition(const vector3f& pos) {
		mT0 = mT1;
		mT1 = pos;

		if ( mInitialMovement ) {
			mInitialMovement = false;
			mT0 = mT1;
		}
	}

private:
	bool		mInitialMovement;
	vector3f	mT0;
	vector3f	mT1;
};

class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

public:
	// IManager implementation
	//{
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);

	PhysicalObject* create(Shape::E shape, float mass);
	PhysicalObject* create(float mass, const vector3f& size, btCollisionShape *shape);
	PhysicalObject* createBox(float mass, const vector3f& size);
	PhysicalObject* createSphere(float mass, float radius);
	bool remove(PhysicalObject *po);

	PhysicalObject* findPhysicalObjectById(Core::AEid id) const;

	void shootBox(const vector3f& position, const vector3f& view);
	//}

	// Connectors
	//{
	void connectConsole(IConsoleInterface *c);
	void connectIdGenerator(Core::IdGenerator *g);
	void connectMediaPathManager(IMediaPathManager *m);
	// }

	// Providers
	// {
	Settings* provideSettings();
	// }

	// debug only
	// {
	void connectObjectManager(Mesh::IObjectManager *m);
	void connectSceneManager(Scene::IManager *m);
	void debugRender();
	void debugSetup();
	// }

	void load();

protected:

private:
	typedef std::list<PhysicalObject*>		ObjectList;
#ifdef USE_BULLET
	typedef std::list<btCollisionShape*>	CollisionShapeList;
	typedef std::list<btMotionState*>		MotionList;
	typedef std::list<btRigidBody*>			RigidBodyList;
#endif

	typedef std::map<Core::AEid, Movement>	MovementMap;
	typedef std::map<Core::AEid, IVolume*>	VolumeMap;

private:
	void generateCollisionGroups();
	void generateCollisionVolumes();
	void trackMovement();

private:
	Settings	mSettings;

	IConsoleInterface	*mConsole;
	Core::IdGenerator	*mIdGenerator;
	IMediaPathManager	*mMediaPathManager;

	ObjectList			mObjects;					// should we use a Core::AEid map instead?
#ifdef USE_BULLET
	CollisionShapeList	mCollisionShapeList;
	MotionList			mMotionList;
	RigidBodyList		mRigidBodyList;

	btBroadphaseInterface				*mBroadphase;
	btDefaultCollisionConfiguration		*mCollisionConfiguration;
	btCollisionDispatcher				*mDispatcher;
	btDiscreteDynamicsWorld				*mDynamicsWorld;
	btSequentialImpulseConstraintSolver	*mSolver;
#endif

	MovementMap	mMovements;
	VolumeMap	mVolumes;
	World		*mWorld;

	// debug only
	// {
	Mesh::IObjectManager	*mObjectManager;
	Scene::IManager			*mSceneManager;
	// }
};


}


#endif
