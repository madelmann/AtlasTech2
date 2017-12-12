
#ifndef _NPC_NPC_h_
#define _NPC_NPC_h_


// Library includes
#include <list>

// Project includes
#include <Interfaces\Core\Types.h>
#include <Math\Vector3.h>

// Forward declarations
namespace Scene {
	class SceneGraphObject;
}

// Namespace declarations


namespace NPCSpace {


class NPC
{
public:
	NPC();
	virtual ~NPC();

public:
	void init(const vector3f& pos, const vector3f& rot);

public:
	float health() const;
	void health(float health);

	vector3f position() const;
	void position(const vector3f& position);

	vector3f rotation() const;
	void rotation(const vector3f& rotation);

	Scene::SceneGraphObject* getSceneGraphObject() const;
	void setSceneGraphObject(Scene::SceneGraphObject *sgo);

protected:

private:
	float					mHealth;
	Scene::SceneGraphObject	*mSceneGraphObject;
};


typedef std::list<NPC*> NPCList;


}


#endif
