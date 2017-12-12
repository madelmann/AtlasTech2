
#ifndef _Scene_Scene_h_
#define _Scene_Scene_h_


// Library includes
#include <list>

// Project includes
#include "SceneGraphObject.h"
#include <Common/Logger.h>
#include <Interfaces/Common/AComponent.h>
#include <Interfaces/Core/Types.h>
#include <Math/Vector3.h>

// Forward declarations
namespace CameraSpace {
	class IViewer;
}
namespace Core {
	class RenderObject;
}
class Frustum;
class IOcclusionQuery;

// Namespace declarations


namespace Scene {

// Forward declarations
class Settings;

class Scene : public SceneGraphObject,
			  public Common::AComponent,
			  private Common::Logger
{
public:
	Scene(const Common::Logger *p);
	~Scene();

public:
	// Common::AComponent implementation
	//{
	void configurationComplete();
	void shutdown();
	//}

	// Connectors
	//{
	void connectFrustum(Frustum *f);
	void connectSettings(Settings *s);
	void connectViewer(CameraSpace::IViewer *v);
	//}

	// SceneGraphObject implementation
	// {
	void render();
	// }

protected:

private:
	float getDistanceToViewer(const vector3f& pos);
	float getFadeValue(SceneGraphObject *sgo);
	bool isInViewingDistance(SceneGraphObject *sgo);

private:
	Frustum					*mFrustum;
	Settings				*mSettings;
	CameraSpace::IViewer	*mViewer;
};


}


#endif
