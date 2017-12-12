
#ifndef Interfaces_Scene_IManager_h
#define Interfaces_Scene_IManager_h


// Library includes

// Project includes
#include <Interfaces/Common/AManager.h>
#include <Interfaces/Core/Types.h>

// Forward declarations
namespace Core {
	class RenderObject;
}
class vector3f;

// Namespace declarations


namespace Scene {

// Forward declarations
class Scene;
class SceneGraphObject;

class IManager : public Common::AManager
{
public:
	virtual ~IManager() { }

public:
	// SceneGraphObject specific
	// {
	virtual void attach(SceneGraphObject *child, Core::AEid parent = Core::invalidID, bool keepPosition = true) = 0;
	virtual void attach(SceneGraphObject *child, SceneGraphObject *parent, bool keepPosition = true) = 0;
	virtual SceneGraphObject* create(Core::RenderObject *ro) = 0;
	virtual void detach(Core::AEid id) = 0;
	virtual void detach(SceneGraphObject *sgo) = 0;
	virtual void erase(SceneGraphObject *sgo) = 0;
	virtual SceneGraphObject* getById(Core::AEid id) = 0;
	virtual SceneGraphObject* getByName(const std::string& name) = 0;
	virtual SceneGraphObject* replicate(SceneGraphObject *sgo) = 0;
	// }

	virtual SceneGraphObject* collidePoint(const vector3f& vector, bool child = false) = 0;

	// Scene specific
	// {
	virtual Scene* getScene() = 0;
	virtual bool load(const std::string& filename) = 0;
	// }
};


}


#endif
