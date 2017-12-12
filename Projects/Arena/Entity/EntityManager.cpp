
// Header
#include "EntityManager.h"

// Project includes
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Scene/IManager.h>
#include "Entity.h"


EntityManager::EntityManager()
: mObjectManager(0),
  mSceneManager(0)
{
}

EntityManager::~EntityManager()
{
	for ( EntityMap::iterator it = mEntities.begin(); it != mEntities.end(); ++it ) {
		delete it->second;
	}
	mEntities.clear();
}

void EntityManager::connectObjectManager(Mesh::IObjectManager *m)
{
	mObjectManager = m;
}

void EntityManager::connectSceneManager(Scene::IManager *m)
{
	mSceneManager = m;
}

void EntityManager::add(Core::AEid id)
{
	Entity *e = new Entity(id);
	e->mMesh = mSceneManager->create(mObjectManager->create("crate"));

	mSceneManager->attach(e->mMesh);

	mEntities.insert(
		std::make_pair(id, e)
	);
}

Entity* EntityManager::getEntity(Core::AEid id) const
{
	EntityMap::const_iterator it = mEntities.find(id);
	if ( it == mEntities.end() ) {
		return 0;
	}

	return it->second;
}

void EntityManager::remove(Core::AEid id)
{
	Entity *e = mEntities[id];
	if ( !e ) {
		return;
	}

	if ( e->mMesh ) {
		mSceneManager->detach(e->mMesh);
		delete e->mMesh;
	}

	mEntities.erase(id);
}

void EntityManager::updatePosition(Core::AEid id, const vector3f& value)
{
	Entity *e = mEntities[id];
	if ( !e ) {
		return;
	}

	e->position(value);
}

void EntityManager::updateRotation(Core::AEid id, const vector3f& value)
{
	Entity *e = mEntities[id];
	if ( !e ) {
		return;
	}

	e->rotation(value);
}
