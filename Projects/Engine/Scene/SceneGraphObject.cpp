
// Header
#include "SceneGraphObject.h"

// Library includes

// Project includes
#include <Interfaces/Driver/IDriver.h>
#include <Interfaces/Scene/IAttribute.h>
#include <Mesh/Volume/Volumes.h>
#include <ObjectiveScript/Script.h>
#include <Renderer/RenderObject.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Scene {

// hack to fix mem leaks, should be removed/resolved
static Mesh::Volume::AABBoxVolume dummyVolume;

SceneGraphObject::SceneGraphObject(Core::AEid id, const std::string& name)
: Core::Object(id, name),
  mAlwaysVisible(false),
  mBoundingVolume(&dummyVolume),
  mCollisionIdentifier(Core::invalidID),
  mEditorMode(0),
  mParent(0),
  mPhysicalBody(0),
  mPositionOrSizeChanged(false),
  mRenderObject(0),
  mScale(1.f, 1.f, 1.f),
  mScript(0),
  mVisible(true)
{
}

SceneGraphObject::~SceneGraphObject()
{
	clear();
}

void SceneGraphObject::addAttribute(IAttribute *a)
{
	if ( !a ) {
		return;
	}

	mAttributes.push_back(a);
}

bool SceneGraphObject::alwaysVisible() const
{
	return mAlwaysVisible;
}

void SceneGraphObject::alwaysVisible(bool state)
{
	mAlwaysVisible = state;
}

void SceneGraphObject::applyAttributes()
{
	for ( AttributeList::const_iterator it = mAttributes.begin(); it != mAttributes.end(); ++it ) {
		(*it)->apply();
	}
}

bool SceneGraphObject::attach(SceneGraphObject *node, bool keepTransformations)
{
	if ( !node ) {
		return false;
	}

	if ( contains(node) ) {
		return false;
	}

	if ( keepTransformations ) {
		// get node's current world coordinates ...
		vector3f v = node->getPosition();
		// ... and subtract our position ...
		v -= mPosition;
		// ... then reset node's position
		node->setPosition(v);

		// get node's current rotation
		v = node->getRotation();
		// ... and subtract our rotation ...
		v -= mRotation;
		// ... then reset node's rotation
		node->setRotation(v);
	}

	node->setEditorMode(mEditorMode);
	node->setParent(this);

	// only resize bounding volume if the newly
	// added node is relative to us
	if ( !keepTransformations ) {
		resizeBoundingVolume(node);
	}

	mNodes.push_back(node);

	return true;
}

//void SceneGraphObject::attachPhysicalBody(PhysicalObject *body)
//{
//	if ( mPhysicalBody ) {
//		assert(!"object already has a physical body attached!");
//	}
//
//	mPhysicalBody = body;
//#ifdef USE_BULLET
//	mPhysicalBody->setSceneGraphObject(this);
//#endif
//}

bool SceneGraphObject::boxCollision(const vector3f& vector, float radius, bool /*force*/)
{
	vector3f me = vector;
	me.x -= mPosition.x;
	me.y -= mPosition.y;
	me.z -= mPosition.z;

	vector3f size = mBoundingVolume->size();

	if ( me.x + radius >= 0.f && me.x - radius <= size.x &&
		me.y + radius >= 0.f && me.y - radius <= size.y &&
		me.z + radius >= 0.f && me.z - radius <= size.z ) {
		return true;
	}

	return false;
}

void SceneGraphObject::clear()
{
	for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
		(*it)->clear();
	}

	mNodes.clear();
}

bool SceneGraphObject::collide(const vector3f& vector, float radius)
{
	switch ( mBoundingVolume->type() ) {
		case IVolume::Type::AABBox:
		case IVolume::Type::AABBoxNoTranslate:
		case IVolume::Type::Box:
			return boxCollision(vector, radius);
		case IVolume::Type::Cylinder:
			return cylinderCollision(vector, radius);
		case IVolume::Type::Sphere:
			return sphereCollision(vector, radius);
		default:
			return boxCollision(vector, radius);
	}
}

Core::AEid SceneGraphObject::collisionIdentifier() const
{
	return mCollisionIdentifier;
}

void SceneGraphObject::collisionIdentifier(Core::AEid value)
{
	mCollisionIdentifier = value;
}

bool SceneGraphObject::contains(SceneGraphObject *node)
{
	if ( !node ) {
		return false;
	}

	for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
		if ( (*it) == node ) {
			return true;
		}
	}

	return false;
}

bool SceneGraphObject::containsVolume(SceneGraphObject *node)
{
	if ( !node ) {
		return false;
	}

	vector3f s1 = mBoundingVolume->size();
	vector3f s2 = node->getBoundingVolume()->size();

	if ( s2.x > s1.x || s2.y > s1.z || s2.z > s1.z ) {
		return false;
	}

	//vector3f p2 = node->getPosition();

	//// 
	//if ( p2.x - s2.x / 2 < 0.f 

	return true;
}

bool SceneGraphObject::cylinderCollision(const vector3f& vector, float radius, bool /*force*/)
{
	vector3f me = vector;
	me.x -= mPosition.x;
	me.y -= mPosition.y;
	me.z -= mPosition.z;

	vector3f size = mBoundingVolume->size();

	if ( me.x + radius >= 0.f && me.x - radius <= size.x &&
		me.z + radius >= 0.f && me.z - radius <= size.z )
	{
		if ( me.y + radius >= 0.f && me.y - radius <= size.y ) {
			return true;
		}
	}

	return false;
}

bool SceneGraphObject::detach(SceneGraphObject *node, bool keepTransformations)
{
	if ( !node ) {
		return false;
	}

	for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
		if ( (*it) == node ) {
			// remove from list ...
			mNodes.remove(node);

			if ( keepTransformations ) {
				// ... transform to world coordinates ...
				vector3f pos = node->getPosition();
				pos += mPosition;
				node->setPosition(pos);
			}

			// ... and don't forget to set the parent entry to 0
			node->setParent(0);

			return true;
		}

		if ( (*it)->detach(node) ) {
			return true;
		}
	}

	return false;
}

SceneGraphObject* SceneGraphObject::find(const std::string name) const
{
	if ( name.empty() ) {
		return 0;
	}

	for ( NodeList::const_iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
		if ( (*it)->name() == name ) {
			return (*it);
		}
	}

	return 0;
}

IVolume* SceneGraphObject::getBoundingVolume() const
{
	return mBoundingVolume;
}

SceneGraphObject* SceneGraphObject::getParent() const
{
	return mParent;
}

//PhysicalObject* SceneGraphObject::getPhysicalObject() const
//{
//	return mPhysicalBody;
//}

const vector3f& SceneGraphObject::getPosition() const
{
	return mPosition;
}

float SceneGraphObject::getRadius() const
{
	return fastSquareRoot(mSize.x * mSize.x + mSize.y * mSize.y + mSize.z * mSize.z) * 0.5f;
}

Core::RenderObject* SceneGraphObject::getRenderObject() const
{
	return mRenderObject;
}

const vector3f& SceneGraphObject::getRotation() const
{
	return mRotation;
}

const vector3f& SceneGraphObject::getScale() const
{
	return mScale;
}

ObjectiveScript::Script* SceneGraphObject::getScript() const
{
	return mScript;
}

const vector3f& SceneGraphObject::getSize() const
{
	return mSize;
}

void SceneGraphObject::hide()
{
	mVisible = false;
}

void SceneGraphObject::init(const vector3f& position, const vector3f& rotation)
{
	mPosition = position;
	mRotation = rotation;

	if ( mRenderObject ) {
		mRotation += mRenderObject->getRotation();
		mSize = mRenderObject->getSize();
	}
}

bool SceneGraphObject::isVisible() const
{
	return mVisible;
}

void SceneGraphObject::render()
{
	if ( !isVisible() ) {
		return;
	}

	aePushMatrix();
		applyAttributes();

		aeTranslate(mPosition);
		aeRotate(mRotation);

		if ( mEditorMode && (*mEditorMode) ) {
			renderBoundingVolume(false);
		}

		if ( mRenderObject ) {
			mRenderObject->render();
		}

		for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
			(*it)->render();
		}
	aePopMatrix();
}

void SceneGraphObject::renderBoundingVolume(bool solid)
{
	if ( !mBoundingVolume ) {
		return;
	}

	aePushMatrix();
/*
		aeTranslate(mPosition);
		aeRotate(mRotation);
*/

		mBoundingVolume->render(solid);

/*
		for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
			if ( mEditorMode && (*mEditorMode) ) {
				(*it)->renderBoundingVolume(solid);
			}
		}
*/
	aePopMatrix();
}

void SceneGraphObject::renderNodes()
{
	aePushMatrix();
		for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
			(*it)->render();
		}
	aePopMatrix();
}

void SceneGraphObject::resizeBoundingVolume(SceneGraphObject *newlyAddedNode)
{
	IVolume* volume = newlyAddedNode->getBoundingVolume();

	if ( !volume ) {
		return;
	}

	vector3f p1 = mPosition;
	vector3f p2 = volume->position();
	//vector3f s1 = mSize;
	vector3f s2 = newlyAddedNode->getBoundingVolume()->size();

	float diff;
	// x low:
	diff = p2.x - s2.x / 2.f;
	if ( diff < 0.f ) {
		mPosition.x -= diff * (-1.f);
		mSize.x += diff * (-1.f);
	}
	// x high:
	diff = p2.x + s2.x / 2.f;
	if ( diff > mSize.x ) {
		mSize.x += diff;
	}

	// y low
	diff = p2.y - s2.y / 2.f;
	if ( diff < 0.f ) {
		mPosition.y -= diff * (-1.f);
		mSize.y += diff * (-1.f);
	}
	// y high:
	diff = p2.y + s2.y / 2.f;
	if ( diff > mSize.y ) {
		mSize.y += diff;
	}

	// z low:
	diff = p2.z - s2.z / 2.f;
	if ( diff < 0.f ) {
		mPosition.z -= diff * (-1.f);
		mSize.z += diff * (-1.f);
	}
	// z high:
	diff = p2.z + s2.z / 2.f;
	if ( diff > mSize.z ) {
		mSize.z += diff;
	}

	mBoundingVolume->size(mSize);


	// move all child nodes about (p1 - mPosition)
	for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
		vector3f old = (*it)->getPosition();
		(*it)->setPosition(old + (p1 - mPosition));
	}
}

void SceneGraphObject::setBoundingVolume(IVolume *volume)
{
	mBoundingVolume = volume;
}

void SceneGraphObject::setEditorMode(bool *mode)
{
	mEditorMode = mode;
}

void SceneGraphObject::setHighLight(const vector3f& color)
{
	mHighLight = color;
}

void SceneGraphObject::setParent(SceneGraphObject *node)
{
	mParent = node;
}

void SceneGraphObject::setPosition(const vector3f& position)
{
	mPosition = position;

#ifdef USE_BULLET
	if ( mPhysicalBody ) {
		mPhysicalBody->setPosition(position);
	}
#endif
}

void SceneGraphObject::setRotation(const vector3f& rotation)
{
	mRotation = rotation;

#ifdef USE_BULLET
	if ( mPhysicalBody ) {
		mPhysicalBody->setRotation(rotation);
	}
#endif
}

void SceneGraphObject::setRenderObject(Core::RenderObject *ro)
{
	if ( mRenderObject ) {
		assert(!"mRenderObject already set!");
		return;
	}

	mRenderObject = ro;

	if ( mRenderObject->getBoundingVolume() ) {
		mBoundingVolume = mRenderObject->getBoundingVolume();
		//mBoundingVolume->translation(mRenderObject->getBoundingVolume()->translation());
		//mBoundingVolume->size(mRenderObject->getBoundingVolume()->size());
	}
}

void SceneGraphObject::setScale(const vector3f& scale)
{
	mScale = scale;

	for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); it++ ) {
		(*it)->setScale(scale);
	}
}

void SceneGraphObject::setScript(ObjectiveScript::Script *s)
{
	mScript = s;
}

void SceneGraphObject::setSize(const vector3f& size)
{
	mSize = size;
}

void SceneGraphObject::show()
{
	mVisible = true;
}

bool SceneGraphObject::sphereCollision(const vector3f& vector, float radius, bool /*force*/)
{
// not correct
	vector3f me = vector;
	me.x -= mPosition.x;
	me.y -= mPosition.y;
	me.z -= mPosition.z;

	vector3f size = mBoundingVolume->size();

	if( me.x + radius >= 0.f && me.x - radius <= size.x &&
		me.z + radius >= 0.f && me.z - radius <= size.z )
	{
		if ( me.y + radius >= 0.f && me.y - radius <= size.y ) {
			return true;
		}
	}

	return false;
}

void SceneGraphObject::update(float elapsedTime)
{
	//updateTransformations(elapsedTime);

	if ( mRenderObject ) {
		mRenderObject->update(elapsedTime);	
	}

	for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); it++ ) {
		(*it)->update(elapsedTime);
	}
}

void SceneGraphObject::updateOcclusionQuery(IDriver *driver)
{
	if ( !mVisible ) {
		return;
	}

	aePushMatrix();
		aeTranslate(mPosition);
		aeRotate(mRotation);

		if ( mRenderObject ) {
			mRenderObject->updateOcclusionQuery(driver);
		}

		for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
			(*it)->updateOcclusionQuery(driver);
		}
	aePopMatrix();
}

//void SceneGraphObject::updateTransformations(float /*elapsedTime*/)
//{
//}


}
