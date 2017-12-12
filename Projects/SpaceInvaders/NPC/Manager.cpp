
// Header
#include "Manager.h"

// Library includes

// Project includes
#include "Boss.h"
#include "Enemy.h"
#include "Player.h"
#include <Collision/Manager.h>
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Scene/SceneGraphObject.h>
#include <Tools/Tools.h>
#include <Types.h>

// Namespace declarations


namespace NPCSpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "NPC::Manager"),
  mCollisionManager(0),
  mMediaPathManager(0),
  mObjectManager(0),
  mSceneManager(0)
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

	for ( NPCList::iterator it = mNPCs.begin(); it != mNPCs.end(); ++it ) {
		mCollisionManager->unregisterSGO((*it)->getSceneGraphObject());
		mSceneManager->erase((*it)->getSceneGraphObject());
		aeDelete((*it));
	}
	mNPCs.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mCollisionManager);
	assert(mMediaPathManager);
	assert(mObjectManager);
	assert(mSceneManager);

	mConfigurationComplete = true;
}

void Manager::connectCollisionManager(Collision::Manager *m)
{
	mCollisionManager = m;
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

NPCList::iterator Manager::find(Scene::SceneGraphObject *npc)
{
	for ( NPCList::iterator it = mNPCs.begin(); it != mNPCs.end(); ++it ) {
		if ( (*it)->getSceneGraphObject() == npc ) {
			return it;
		}
	}

	return mNPCs.end();
}

bool Manager::hit(Scene::SceneGraphObject *npc)
{
	if ( !npc ) {
		return false;
	}

	NPCList::iterator it = find(npc);
	if ( it == mNPCs.end() ) {
		// this SceneGraphObject is not one of our npcs
		return false;
	}

	(*it)->health((*it)->health() - 1);
	if ( (*it)->health() <= 0 ) {
		killNPC((*it));
	}

	return true;
}

void Manager::init()
{
	debug("Initializing...");

	loadConfig();

	mIsReady = true;
}

bool Manager::isThereAnEnemyStillAlive() const
{
	return !mNPCs.empty();
}

void Manager::killNPC(NPCSpace::NPC *npc)
{
	// our npc has been killed,
	// so remove all garbage
	mCollisionManager->unregisterSGO(npc->getSceneGraphObject());
	mSceneManager->erase(npc->getSceneGraphObject());

	mNPCs.remove(npc);
}

void Manager::loadConfig()
{
	std::string file = mMediaPathManager->buildConfigPath("npc.conf");

	Core::Configuration::Configurator config(this, file);
	if ( !config.success() ) {
		warn("invalid configuration file '" + file + "'!");
		return;
	}

	// Enemy setup
	// {
	int enemy_count = config.getValue("enemy.size").toInt();
	for ( int c = 0; c < enemy_count; ++c ) {
		float health = config.getValue("enemy:" + Tools::toString(c) + ".health").toFloat();
		std::string mesh = config.getValue("enemy:" + Tools::toString(c) + ".mesh").toString();
		vector2f pos = config.getValue("enemy:" + Tools::toString(c) + ".position").toVector2f();

		Core::RenderObject *enemy = mObjectManager->create(mesh);

		Scene::SceneGraphObject *sgo = mSceneManager->create(enemy);
		sgo->collisionIdentifier(EnemyShip);
		mSceneManager->attach(sgo, Core::invalidID, true);

		mCollisionManager->registerSGO(sgo);

		NPC *npc = new Enemy();
		npc->setSceneGraphObject(sgo);
		npc->health(health);
		npc->position(vector3f(pos.x, 0.f, pos.y));

		mNPCs.push_back(npc);
	}
	// }

	mSettings.movement(config.getValue("npc.movement").toVector3f());
	mSettings.speed(config.getValue("npc.speed").toFloat());

	//// Player setup
	//// {
	//float health = config.getValue("player.health").toFloat();
	//std::string mesh = config.getValue("player.mesh").toString();
	//vector2f pos = config.getValue("player.position").toVector2f();

	//Core::RenderObject *player = mObjectManager->create(mesh);

	//Scene::SceneGraphObject *sgo = mSceneManager->add(player);
	//sgo->collisionIdentifier(PlayerShip);
	//mSceneManager->attach(sgo);

	//mCollisionManager->registerSGO(sgo);

	//NPC *npc = new Player();
	//npc->setSceneGraphObject(sgo);
	//npc->health(health);
	//npc->position(vector3f(pos.x, 0.f, pos.y));

	//mNPCs.push_back(npc);
	//// }
}

void Manager::manage()
{
	// nothing to do here
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::update(float elapsedTime)
{
	static vector3f pos;
	static int dir = 1;

	float delta = dir * mSettings.speed() * elapsedTime;
	pos.x += delta;
	pos.y = 0;
	pos.z = 0;

	if ( pos.x <= mSettings.movement().x ) {
		dir = 1;
		pos.z = mSettings.movement().z;
	}
	else if ( pos.x >= mSettings.movement().y ) {
		dir = -1;
		pos.z = mSettings.movement().z;
	}

	for ( NPCList::iterator it = mNPCs.begin(); it != mNPCs.end(); ++it ) {
		vector3f position = (*it)->position();
		position.x += delta;
		position.z -= pos.z;
		(*it)->position(position);
	}
}


}
