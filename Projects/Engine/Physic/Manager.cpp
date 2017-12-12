
#pragma warning(disable:4100)
#pragma warning(disable:4127)

// Header
#include "Manager.h"

// Library includes
#ifdef USE_BULLET
#include <BulletOpenGL/include/GLDebugDrawer.h>
#endif

// Project includes
#ifdef USE_BULLET
#include "BulletDebugRenderer.h"
#include "MotionState.h"
#include "PhysicalObject.h"
#endif
#include "ConsolePlugin.h"
#include "World.h"
#include <Core/Configuration/Configurator.h>
#include <Core/IdGenerator.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Mesh/Format/VolumeMesh/BoxMesh.h>
#include <Mesh/Format/VolumeMesh/SphereMesh.h>
#include <Mesh/Volume/AABBox.h>
#include <Parsers/LineParser.h>
#include <Tools/Strings.h>

// Namespace declarations


///create 125 (5x5x5) dynamic object
#define ARRAY_SIZE_X 8
#define ARRAY_SIZE_Y 1
#define ARRAY_SIZE_Z 8

//maximum number of objects (and allow user to shoot additional boxes)
#define MAX_PROXIES (ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z + 1024)

///scaling of the objects (0.1 = 20 centimeter boxes )
#define SCALING 1.
#define START_POS_X 0
#define START_POS_Y 512
#define START_POS_Z 0


namespace Physics {


#ifdef USE_BULLET
static BulletDebugRenderer gDebugDrawer;
#endif

Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "Physics::Manager"),
  mConsole(0),
 #ifdef USE_BULLET
  mBroadphase(0),
  mCollisionConfiguration(0),
  mDispatcher(0),
  mDynamicsWorld(0),
  mSolver(0),
#endif
  mIdGenerator(0),
  mObjectManager(0),
  mSceneManager(0),
  mWorld(0)
{
	debug("Starting...");
}

Manager::~Manager()
{
	shutdown();
}

void Manager::clear()
{
	debug("Clearing...");

	mSettings.clear();

	aeDelete(mWorld);

#ifdef USE_BULLET
	for ( ObjectList::iterator it = mObjects.begin(); it != mObjects.end(); ++it ) {
		delete (*it);
	}
	mObjects.clear();

	for ( CollisionShapeList::iterator it = mCollisionShapeList.begin(); it != mCollisionShapeList.end(); ++it ) {
		delete (*it);
	}
	mCollisionShapeList.clear();

	for ( RigidBodyList::iterator it = mRigidBodyList.begin(); it != mRigidBodyList.end(); ++it ) {
		mDynamicsWorld->removeRigidBody((*it));
		delete (*it);
	}
	mRigidBodyList.clear();

	for ( MotionList::iterator it = mMotionList.begin(); it != mMotionList.end(); ++it ) {
		delete (*it);
	}
	mMotionList.clear();
#endif
}

void Manager::configurationComplete()
{
	assert(!mConfigurationComplete);

#ifdef USE_BULLET
	mBroadphase = new btDbvtBroadphase();
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mSolver = new btSequentialImpulseConstraintSolver;
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);

	assert(mBroadphase);
	assert(mCollisionConfiguration);
	assert(mDispatcher);
	assert(mDynamicsWorld);
	assert(mSolver);
#endif

	assert(mConsole);
	assert(mIdGenerator);
	assert(mMediaPathManager);

	ConsolePlugin *plugin = new ConsolePlugin();
	plugin->connectPhysicsManager(this);

	mConsole->registerPlugin(plugin);

#ifdef USE_BULLET
	mDynamicsWorld->setDebugDrawer(&gDebugDrawer);
#endif

	mConfigurationComplete = true;
}

void Manager::connectConsole(IConsoleInterface *c)
{
	mConsole = c;
}

void Manager::connectIdGenerator(Core::IdGenerator *g)
{
	mIdGenerator = g;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void Manager::connectObjectManager(Mesh::IObjectManager *m)
{
	mObjectManager = m;
}

void Manager::connectSceneManager(Scene::IManager *m)
{
	mSceneManager = m;
}

PhysicalObject* Manager::create(Shape::E shape, float mass)
{
/*
	PhysicalObject* po = new PhysicalObject(
		mIdGenerator->generate(),
#ifdef USE_BULLET
		mDynamicsWorld,
#else
		0
#endif
		shape,
		mass
	);

	switch ( shape ) {
		case Shape::Capsule:
			break;
		case Shape::Cone:
			break;
		case Shape::ConvexInternalAabbCaching:
			break;
		case Shape::Cylinder:
			break;
		case Shape::MinkowskiSum:
			break;
		case Shape::PolyhedralConvex:
			break;
	}

	mObjects.push_back(po);

	return po;
*/
	return 0;
}

PhysicalObject* Manager::create(float mass, const vector3f& size, btCollisionShape *shape)
{
/*
	PhysicalObject* po = create(Shape::ConvexInternalAabbCaching, mass);

	po->size(size);

#ifdef USE_BULLET
	btVector3 fallInertia(size.x / 2.f, size.y / 2.f, size.z / 2.f);
	shape->calculateLocalInertia(mass, fallInertia);

	po->collisionShape(shape);
	mCollisionShapeList.push_back(shape);
#endif

	return po;
*/
	return 0;
}

PhysicalObject* Manager::createBox(float mass, const vector3f& size)
{
/*
	PhysicalObject* po = create(Shape::ConvexInternalAabbCaching, mass);

	po->size(size);

#ifdef USE_BULLET
	btVector3 sizeV3(size.x, size.y, size.z);
	btBoxShape *shape = new btBoxShape(sizeV3);

	btVector3 fallInertia(size.x / 2.f, size.y / 2.f, size.z / 2.f);
	shape->calculateLocalInertia(mass, fallInertia);

	po->collisionShape(shape);
	mCollisionShapeList.push_back(shape);
#endif

	return po;
*/
	return 0;
}

PhysicalObject* Manager::createSphere(float mass, float radius)
{
	PhysicalObject* po = create(Shape::Sphere, mass);

#ifdef USE_BULLET
	btConvexInternalShape *shape = new btSphereShape(radius);

	btVector3 fallInertia(radius, radius, radius);
	shape->calculateLocalInertia(mass, fallInertia);

	po->collisionShape(shape);
	mCollisionShapeList.push_back(shape);
#endif

	return po;
}

void Manager::debugRender()
{
	if ( mSettings.debugMode() ) {
#ifdef USE_BULLET
		mDynamicsWorld->debugDrawWorld();
#endif
	}
}

void Manager::debugSetup()
{
#ifdef USE_BULLET
	Core::RenderObject *box = mObjectManager->create("BoxMesh");
	assert(box);
	static_cast<Mesh::BoxMesh*>(box)->resize(vector3f(10.f, 10.f, 10.f));

	// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	float start_x = START_POS_X;
	float start_y = START_POS_Y;
	float start_z = START_POS_Z;

	for ( int k = 0; k < ARRAY_SIZE_Y; k++ ) {
		for ( int i = 0; i < ARRAY_SIZE_X; i++ ) {
			for( int j = 0; j < ARRAY_SIZE_Z; j++ ) {
				btVector3 origin = SCALING * btVector3(btScalar(512.0 * i + start_x), btScalar(128.0 * k + start_y), btScalar(512.0 * j + start_z));
				startTransform.setOrigin(origin);

				Scene::SceneGraphObject *box_sgo = mSceneManager->add(box);

				Physics::PhysicalObject *box_po = createBox(10.f, box_sgo->getBoundingVolume()->size());
				box_po->setCollisionGroup(CollisionType::All);
				box_po->setCollisionMask(CollideWithOther);

				box_sgo->attachPhysicalBody(box_po);
				box_sgo->init(vector3f(origin.x(), origin.y(), origin.z()), vector3f());

				mSceneManager->attach(box_sgo, Core::invalidID, true);
			}
		}
	}
#endif
}

PhysicalObject* Manager::findPhysicalObjectById(Core::AEid id) const
{
/*
	for ( ObjectList::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it ) {
		if ( (*it)->id() == id ) {
			return (*it);
		}
	}

	return 0;
*/
	return 0;
}

void Manager::generateCollisionGroups()
{
	// generate groups of all objects that collide
	// then set the collision group id of each object

	//mWorld->subdivide(mSettings.subdivisions());
}

void Manager::generateCollisionVolumes()
{
	for ( MovementMap::const_iterator it = mMovements.begin(); it != mMovements.end(); ++it ) {
		PhysicalObject *po = findPhysicalObjectById((*it).first);
		if ( !po ) {
			// no PhysicalObject found for this id
			// we should delete it from our movement map (but somewhere else)
			continue;
		}

		IVolume *volume = 0;
/*
		switch ( po->getShapeType() ) {
			case Shape::ConvexInternalAabbCaching:
				//volume = new Mesh::Volume::AABBoxVolume(po->size());
				//break;
			default:
				volume = new Mesh::Volume::AABBoxVolume(po->size());
				break;
		}
*/
		mVolumes[(*it).first] = volume;
	}
}

void Manager::init()
{
	debug("Initializing...");

#ifdef USE_BULLET
	btStaticPlaneShape *shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	mCollisionShapeList.push_back(shape);

	btDefaultMotionState *motion = new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0))
	);
	mMotionList.push_back(motion);

	btRigidBody *body = new btRigidBody(
		btRigidBody::btRigidBodyConstructionInfo(0, motion, shape, btVector3(0, 0, 0))
	);
	mRigidBodyList.push_back(body);

	mDynamicsWorld->addRigidBody(body);

	gDebugDrawer.setDebugMode(GLDebugDrawer::DBG_FastWireframe);
#endif

	mWorld = new World(mSettings.position(), mSettings.size());

	mIsReady = true;
}

void Manager::load()
{
	std::string file = mMediaPathManager->buildConfigPath("physics.conf");

	Core::Configuration::Configurator config(this, file);
	if ( !config.success() ) {
		return;
	}

	mSettings.debugMode(config.getValue("debug").toBool());
	mSettings.enabled(config.getValue("enabled").toBool());
	mSettings.gravity(config.getValue("gravity").toVector3f());
	mSettings.subdivisions(config.getValue("subdivisions").toVector3f());
}

void Manager::manage()
{
	static bool alternate = false;
	alternate = !alternate;	// only update every other frame

	if ( alternate ) {
		// update position "database"
		trackMovement();

		// generate our collision volumes
		generateCollisionVolumes();

		// create our collision groups by checking our collision volumes
		generateCollisionGroups();
		
		// don't forget to delete our volumes
		for ( VolumeMap::iterator it = mVolumes.begin(); it != mVolumes.end(); ++it ) {
			delete (*it).second;
		}
		mVolumes.clear();
	}
}

Settings* Manager::provideSettings()
{
	return &mSettings;
}

bool Manager::remove(PhysicalObject *po)
{
	if ( !po ) {
		//error("invalid <PhysicalObject> provided - aborting...");
		return false;
	}

	//mCollisionShapeList.remove(po->getCollisionShape());		// DEPRECATED: this should not removed, because in the future shapes will be shared amoung physical objects to save resources
	//mMotionList.remove(po->getMotionState());
	//mRigidBodyList.remove(po->getRigidBody());
	//mObjects.erase(po);

	return true;
}

void Manager::shootBox(const vector3f& position, const vector3f& view)
{
	Core::RenderObject *box = mObjectManager->create("crate.ao");
	assert(box);
	//static_cast<Mesh::BoxMesh*>(box)->resize(vector3f(10.f, 10.f, 10.f));

#ifdef USE_BULLET
	// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 origin = btVector3(position.x, position.y, position.z);
	startTransform.setOrigin(origin);

	Physics::PhysicalObject *box_po = createBox(10.f, box->getBoundingVolume()->size());
	box_po->setCollisionGroup(CollisionType::All);
	box_po->setCollisionMask(CollideWithOther);
#endif

	Scene::SceneGraphObject *box_sgo = mSceneManager->create(box);
#ifdef USE_BULLET
	box_sgo->attachPhysicalBody(box_po);
	box_sgo->init(position, vector3f());

	vector3f linearVel(view);
	linearVel -= position;
	linearVel.normalize();
	box_po->linearVelocity(linearVel * 66.6f);
#endif

	mSceneManager->attach(box_sgo, Core::invalidID, true);
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();

#ifdef USE_BULLET
	delete mDynamicsWorld;
	delete mSolver;
	delete mCollisionConfiguration;
	delete mDispatcher;
	delete mBroadphase;
#endif
}

void Manager::trackMovement()
{
/*
	for ( ObjectList::iterator it = mObjects.begin(); it != mObjects.end(); ++it ) {
		mMovements[(*it)->id()].updatePosition((*it)->getPosition());
	}
*/
}

void Manager::update(float elapsedTime)
{
	if ( mSettings.hasChanged() ) {
#ifdef USE_BULLET
		vector3f g = mSettings.gravity();
		mDynamicsWorld->setGravity(btVector3(g.x, g.y, g.z));
#endif
	}

	if ( mSettings.enabled() ) {
#ifdef USE_BULLET
		//mDynamicsWorld->stepSimulation(1/60.f, 10);
		mDynamicsWorld->stepSimulation(elapsedTime, 10);
#endif
	}
}


}
