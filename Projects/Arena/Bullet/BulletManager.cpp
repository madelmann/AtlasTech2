
// Header
#include "BulletManager.h"

// Library includes

// Project includes
#include <Collision/Manager.h>
#include <Core/Configuration/Configurator.h>
#include <Core/IdGenerator.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Mesh/IVolume.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Renderer/RenderObject.h>
#include <Scene/SceneGraphObject.h>
#include <Types.h>
#include "Bullet.h"

// Namespace declarations
using TextureSpace::Texture;


BulletManager::BulletManager(const Common::Logger *p)
: Common::Logger(p, "BulletManager"),
  mCollisionManager(0),
  mIdGenerator(0),
  mObjectManager(0),
  mSceneManager(0)
{
	debug("Starting...");
}

BulletManager::~BulletManager()
{
	shutdown();
}

void BulletManager::add(const std::string& name, const vector3f& pos, const vector3f& rot)
{
	Bullet *bullet = create(name);
	if ( !bullet ) {
		return;
	}

	bullet->init(pos, rot);

	mSceneManager->attach(bullet->getSceneGraphObject());
	mCollisionManager->registerSGO(bullet->getSceneGraphObject());
}

Bullet* BulletManager::create(const std::string& name)
{
	if ( name.empty() ) {
		return 0;
	}

	std::string filename = Tools::Files::RemoveFileExt("base/weapons/" + name) + ".bullet";

	Bullet *original = find(name);
	if ( !original ) {
		Core::Configuration::Configurator config(this, filename);
		if ( !config.success() ) {
			return 0;
		}

		original = new Bullet(mIdGenerator->generate(), name);
		{
			original->setDamage(config.getValue("damage").toFloat());
			original->setDirection(vector3f(0.f, 0.f, 1.f));
			original->setLifeTime(config.getValue("lifetime").toFloat());
			original->setSpeed(config.getValue("speed").toFloat());
		}

		mBulletRepository.push_back(original);

		Scene::SceneGraphObject *sgo = mSceneManager->create(
			mObjectManager->create(config.getValue("model").toString())
		);
		sgo->collisionIdentifier(PlayerBullet);
		original->setSceneGraphObject(sgo);
	}

	Bullet* copy = new Bullet(mIdGenerator->generate(), name);
	copy->setSceneGraphObject(
		mSceneManager->replicate(original->getSceneGraphObject())
	);
	mBullets.push_back(copy);

	return copy;
}

void BulletManager::clear()
{
	debug("Clearing...");

	for ( BulletList::iterator it = mBulletRepository.begin(); it != mBulletRepository.end(); it++ ) {
		aeDelete( (*it) );
	}
	mBulletRepository.clear();

	for ( BulletList::iterator it = mBullets.begin(); it != mBullets.end(); it++ ) {
		mCollisionManager->unregisterSGO((*it)->getSceneGraphObject());
		aeDelete( (*it) );
	}
	mBullets.clear();

	mIsReady = false;
}

void BulletManager::configurationComplete()
{
	assert(!mConfigurationComplete);

	assert(mCollisionManager);
	assert(mIdGenerator);
	assert(mObjectManager);
	assert(mSceneManager);

	mConfigurationComplete = true;
}

void BulletManager::connectCollisionManager(Collision::Manager *m)
{
	mCollisionManager = m;
}

void BulletManager::connectIdGenerator(Core::IdGenerator *g)
{
	mIdGenerator = g;
}

void BulletManager::connectObjectManager(Mesh::IObjectManager *m)
{
	mObjectManager = m;
}

void BulletManager::connectSceneManager(Scene::IManager *m)
{
	mSceneManager = m;
}

Bullet* BulletManager::find(const std::string& name) const
{
	for ( BulletList::const_iterator it = mBulletRepository.begin(); it != mBulletRepository.end(); ++it ) {
		if ( (*it)->name() == name ) {
			return (*it);
		}
	}

	return 0;
}

void BulletManager::init()
{
	debug("Initializing...");

	mIsReady = true;
}

void BulletManager::manage()
{
	// create temporary list
	BulletList toRemove;

	for ( BulletList::iterator it = mBullets.begin(); it != mBullets.end(); ++it ) {
		if ( (*it)->getLifeTime() <= 0.f ) {
			// push deleteable bullets to temporary list
			toRemove.push_back((*it));
		}
	}

	for ( BulletList::iterator it = toRemove.begin(); it != toRemove.end(); ++it ) {
		// remove all entries from the outside
		// to avoid problems during iteration ...
		mBullets.remove((*it));

		// .. unregister from all managers ...
		if ( (*it)->getSceneGraphObject() ) {
			mCollisionManager->unregisterSGO((*it)->getSceneGraphObject());

			mSceneManager->erase((*it)->getSceneGraphObject());
		}

		// ... and really delete them
		aeDelete( (*it) );
	}
	toRemove.clear();
}

void BulletManager::removeReference(Scene::SceneGraphObject *sgo)
{
	if ( !sgo ) {
		return;
	}

	// Find our corresponding bullet ..
	Bullet *toRemove = 0;
	for ( BulletList::iterator it = mBullets.begin(); it != mBullets.end(); ++it ) {
		if ( (*it)->getSceneGraphObject() == sgo ) {
			toRemove = (*it);
			break;
		}
	}

	// .. if we found anything ..
	if ( toRemove ) {
		// .. remove it from internal list ..
		mBullets.remove(toRemove);
		// .. and delete it
		aeDelete(toRemove);
	}
}

void BulletManager::shutdown()
{
	debug("Stopping...");

	clear();
}


void BulletManager::update(float elapsedTime)
{
	for ( BulletList::iterator it = mBullets.begin(); it != mBullets.end(); ++it ) {
		(*it)->update(elapsedTime);
	}
}
