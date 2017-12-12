
// Header
#include "BulletManager.h"

// Library includes

// Project includes
#include "Bullet.h"
#include <Types.h>
#include <Collision/Manager.h>
#include <Core/IdGenerator.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Mesh/IVolume.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Renderer/RenderObject.h>
#include <Scene/SceneGraphObject.h>
#include <Texture/Texture.h>

// Namespace declarations
using TextureSpace::Texture;


BulletManager::BulletManager(const Common::Logger *p)
: Common::Logger(p, "BulletManager"),
  mCollisionManager(0),
  mIdGenerator(0),
  mObjectManager(0),
  mSceneManager(0),
  mTextureManager(0)
{
	debug("Starting...");
}

BulletManager::~BulletManager()
{
	shutdown();
}

void BulletManager::add(const vector3f& pos, const vector3f& rot)
{
	Core::RenderObject *ro = mObjectManager->create("bullet.ao");
	if ( !ro ) {
		error("error while loading bullet mesh!");
		return;
	}

	vector3f rotation = ro->getRotation();
	rotation += rot;

	vector3f size = ro->getSize();

	Scene::SceneGraphObject *bullet_sgo = mSceneManager->create(ro);
	bullet_sgo->collisionIdentifier(PlayerBullet);
	mSceneManager->attach(bullet_sgo);


	Bullet *b = new Bullet(mIdGenerator->generate());
	{
		b->setDirection(vector3f(0.f, 0.f, 1.f));
		b->setLifeTime(5.f);
		b->setSceneGraphObject(bullet_sgo);
		b->init(vector3f(vector3f(pos) + size * 0.5f), rotation);
		b->setSpeed(100.f);
	}
	mBullets.push_back(b);

	mCollisionManager->registerSGO(b->getSceneGraphObject());
}

void BulletManager::clear()
{
	debug("Clearing...");

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
	assert(mTextureManager);

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

void BulletManager::connectTextureManager(TextureSpace::IManager *m)
{
	mTextureManager = m;
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
