
// Header
#include "MotionState.h"

// Library includes

// Project includes

// Namespace declarations


namespace Physics {


AEMotionState::AEMotionState(const btTransform &initialpos, SceneGraphObject *node)
: mNode(node),
  mTransform(initialpos)
{
}

AEMotionState::~AEMotionState()
{
}

void AEMotionState::getWorldTransform(btTransform &worldTrans) const
{
	worldTrans = mTransform;
}

void AEMotionState::setNode(SceneGraphObject *node)
{
	mNode = node;
}

void AEMotionState::setWorldTransform(const btTransform &worldTrans)
{
	if ( !mNode ) {
		return; // silently return before we set a node
	}

	btQuaternion rot = worldTrans.getRotation();
	mNode->setRotation(vector3f(rot.x(), rot.y(), rot.z()));
	btVector3 pos = worldTrans.getOrigin();
	mNode->setPosition(vector3f(pos.x(), pos.y(), pos.z()));
}


}
