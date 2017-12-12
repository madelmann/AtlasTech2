
// Header
#include "GeneralManager.h"

// Library includes

// Project includes

// Namespace declarations


namespace Core {


bool sortDependency(Common::AManager *first, Common::AManager *second)
{
	return first->dependsOn(second);
}


GeneralManager::GeneralManager(const Common::Logger *p)
: Common::Logger(p, "GeneralManager")
{
	debug("Starting...");
}

GeneralManager::~GeneralManager()
{
	shutdown();
}

void GeneralManager::add(AManager *m)
{
	mManagers.push_back(m);
}

void GeneralManager::clear()
{
	debug("Clearing...");

	for ( ManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		if ( (*it) && (*it)->isReady() ) {
			(*it)->clear();
		}
	}
}

void GeneralManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	for ( ManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		(*it)->configurationComplete();
	}

	mConfigurationComplete = true;
}

void GeneralManager::init()
{
	debug("Initializing...");

	for ( ManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		(*it)->init();
	}

	sort();

	mIsReady = true;
}

void GeneralManager::manage()
{
	for ( ManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		(*it)->manage();
	}
}

void GeneralManager::reset()
{
	debug("Reseting...");

	for ( ManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		if ( (*it) && (*it)->isReady() ) {
			(*it)->reset();
		}
	}
}

void GeneralManager::shutdown()
{
	debug("Stopping...");

	// do not clear manager contents because they maybe have already been deleted!
	//clear();

	mManagers.clear();
}

void GeneralManager::sort()
{
	debug("Sorting...");

	mManagers.sort(sortDependency);
}

void GeneralManager::update(float elapsedTime)
{
	for ( ManagerList::iterator it = mManagers.begin(); it != mManagers.end(); it++ ) {
		(*it)->update(elapsedTime);
	}
}


}
