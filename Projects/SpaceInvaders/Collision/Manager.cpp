
// Header
#include "Manager.h"

// Library includes

// Project includes
#include <Scene\SceneGraphObject.h>
#include <Tools\Tools.h>

// Namespace declarations


namespace Collision {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "CollisionManager")
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

	mRegisteredSGOs.clear();

	mIsReady = false;
}

void Manager::configurationComplete()
{
	assert(!mConfigurationComplete);



	mConfigurationComplete = true;
}

Manager::SGOList::iterator Manager::find(Scene::SceneGraphObject *sgo)
{
	for ( SGOList::iterator it = mRegisteredSGOs.begin(); it != mRegisteredSGOs.end(); ++it ) {
		if ( (*it) == sgo ) {
			return it;
		}
	}

	return SGOList::iterator();
}

const Physics::CollisionGroupsList& Manager::getCollisionGroups()
{
	updateCollisionGroups();

	return mCollisionGroups;
}

void Manager::init()
{
	debug("Initializing...");

	mIsReady = true;
}

void Manager::manage()
{
	// nothing to do here
}

void Manager::registerSGO(Scene::SceneGraphObject *sgo)
{
	if ( !sgo ) {
		warn("invalid <SceneGraphObject> provided!");
		return;
	}

	//if ( find(sgo) != mRegisteredSGOs.end() ) {
	//	warn("already registered <SceneGraphObject> found!");
	//	return;
	//}

	mRegisteredSGOs.push_back(sgo);
}

void Manager::updateCollisionGroups()
{
	// reset our collision groups
	mCollisionGroups.clear();

	for ( SGOList::iterator out = mRegisteredSGOs.begin(); out != mRegisteredSGOs.end(); ++out ) {
		if ( (*out)->collisionIdentifier() == Core::invalidID ) {
			// no collision identifier set => do not collide with this SGO
			continue;
		}

		Physics::CollisionGroup g;
		g.add((*out)->id());

		for ( SGOList::iterator in = mRegisteredSGOs.begin(); in != mRegisteredSGOs.end(); ++in ) {
			if ( (*out) == (*in) ) {
				// do not try to collide with ourself
				continue;
			}
			if ( (*in)->collisionIdentifier() == Core::invalidID ) {
				// no collision identifier set => do not collide with this SGO
				continue;
			}

			bool result = (*out)->collide((*in)->getPosition(), (*in)->getBoundingVolume()->radius());
			if ( result ) {
				// a collision occured => add this SGO to our collision group
				g.add((*in)->id());
			}
		}

		// only add to our groups list if we contain more than 1 element
		if ( g.size() > 1 ) {
			mCollisionGroups.push_back(g);
		}
	}
}

void Manager::unregisterSGO(Scene::SceneGraphObject *sgo)
{
	if ( !sgo ) {
		warn("invalid <SceneGraphObject> provided!");
		return;
	}

	if ( find(sgo) == mRegisteredSGOs.end() ) {
		warn("unregistered <SceneGraphObject> found!");
		return;
	}

	mRegisteredSGOs.remove(sgo);
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::update(float /*elapsedTime*/)
{
	// nothing to do here
}


}
