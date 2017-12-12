
#ifndef _Physics_MotionState_h_
#define _Physics_MotionState_h_


// Library includes
#include <btBulletDynamicsCommon.h>

// Project includes
#include <Scene\SceneGraphObject.h>

// Forward declarations

// Namespace declarations
using Scene::SceneGraphObject;


namespace Physics {


class AEMotionState : public btMotionState 
{
public:
	AEMotionState(const btTransform &initialpos, SceneGraphObject *node);
	virtual ~AEMotionState();

	void setNode(SceneGraphObject *node);

	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);

protected:
	SceneGraphObject	*mNode;
    btTransform			mTransform;
};


}


#endif
