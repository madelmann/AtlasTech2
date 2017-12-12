
#ifndef Arena_Entity_Entity_h
#define Arena_Entity_Entity_h


// Library includes

// Project includes
#include <Interfaces/Core/Id.h>
#include <Math/Vector3.h>
#include <Renderer/RenderObject.h>
#include <Scene/SceneGraphObject.h>


class Entity : public Core::Id
{
public:
	Entity(Core::AEid id);
	virtual ~Entity();

public:
	vector3f direction() const;
	void direction(const vector3f& value);

	vector3f position() const;
	void position(const vector3f& value);

	vector3f rotation() const;
	void rotation(const vector3f& value);

	Scene::SceneGraphObject *mMesh;

protected:

private:

};


#endif
