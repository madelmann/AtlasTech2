
#ifndef _Scene_SceneGraphObject_h_
#define _Scene_SceneGraphObject_h_


// Library includes
#include <list>

// Project includes
//#include <Interfaces/Core/Id.h>
#include <Interfaces/Core/Object.h>
#include <Interfaces/Mesh/IVolume.h>
#include <Math/Vector3.h>

// Forward declarations
namespace Core {
	class RenderObject;
}
class IDriver;
namespace ObjectiveScript {
	class Script;
}
namespace Physics {
	class PhysicalObject;
}

// Namespace declarations
using Physics::PhysicalObject;

namespace Scene {

// Forward declarations
class IAttribute;

class SceneGraphObject : public Core::Object
{
public:
	class Type
	{
	public:
		enum E {
			Leaf,
			Node,
			Occluder
		};
	};

public:
	SceneGraphObject(Core::AEid id = Core::invalidID, const std::string& name = "");
	~SceneGraphObject();

public:
	// Node specific
	// {
	void addAttribute(IAttribute *a);
	bool attach(SceneGraphObject *node, bool keepTransformations = false);
	void clear();
	bool contains(SceneGraphObject *node);
	bool detach(SceneGraphObject *node, bool keepTransformations = true);
	SceneGraphObject* find(const std::string name) const;

	SceneGraphObject* getParent() const;
	void setParent(SceneGraphObject* node);
	// }

	// Renderer specific
	// {
	virtual void render();
	virtual void renderBoundingVolume(bool solid = false);
	virtual void renderNodes();

	void update(float elapsedTime = 0.f);
	void updateOcclusionQuery(IDriver *driver);
	// }

	IVolume* getBoundingVolume() const;
	void setBoundingVolume(IVolume *volume);

	virtual bool collide(const vector3f& vector, float radius);

	virtual bool boxCollision(const vector3f& vector, float radius, bool force = false);
	virtual bool cylinderCollision(const vector3f&  vector, float radius, bool force = false);
	virtual bool sphereCollision(const vector3f&  vector, float radius, bool force = false);

	bool alwaysVisible() const;
	void alwaysVisible(bool state);

	//PhysicalObject* getPhysicalObject() const;

	const vector3f& getPosition() const;
	void setPosition(const vector3f& position);

	float getRadius() const;

	Core::RenderObject* getRenderObject() const;
	void setRenderObject(Core::RenderObject *ro);

	const vector3f& getRotation() const;
	void setRotation(const vector3f& rotation);

	const vector3f& getScale() const;
	void setScale(const vector3f& scale);

	ObjectiveScript::Script* getScript() const;
	void setScript(ObjectiveScript::Script *s);

	const vector3f& getSize() const;
	void setSize(const vector3f& size);

	// Physics
	// {
	Core::AEid collisionIdentifier() const;
	void collisionIdentifier(Core::AEid value);
	// }

	// Visibility
	// {
	bool isVisible() const;

	void hide();
	void show();
	// }

	//void attachPhysicalBody(PhysicalObject *body);
	void init(const vector3f& position, const vector3f& rotation);

	// Debug
	// {
	void setEditorMode(bool *mode);
	void setHighLight(const vector3f& color);
	// }

protected:
	typedef std::list<IAttribute*> AttributeList;
	typedef std::list<SceneGraphObject*> NodeList;

protected:
	bool containsVolume(SceneGraphObject *volume);
	void resizeBoundingVolume(SceneGraphObject *newlyAddedNode);
	//void updateTransformations(float elapsedTime = 0.f);

protected:
	bool				*mEditorMode;
	NodeList			mNodes;
	SceneGraphObject	*mParent;
	PhysicalObject		*mPhysicalBody;
	Core::RenderObject	*mRenderObject;
	ObjectiveScript::Script	*mScript;

protected:
	bool			mAlwaysVisible;
	AttributeList	mAttributes;
	IVolume			*mBoundingVolume;
	vector3f		mPosition;
	bool			mPositionOrSizeChanged;
	vector3f		mRotation;
	vector3f		mScale;
	vector3f		mSize;
	bool			mVisible;

private:
	void applyAttributes();

private:
	vector3f	mHighLight;

private:	// Physics
	Core::AEid	mCollisionIdentifier;
};


}


#endif
