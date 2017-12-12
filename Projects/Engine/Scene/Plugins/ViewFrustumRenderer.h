
#ifndef _Scene_Plugins_ViewFrustumRenderer_h_
#define _Scene_Plugins_ViewFrustumRenderer_h_


// Library includes

// Project includes
#include <Interfaces/Renderer/IRendererPlugin.h>

// Forward declarations
class Frustum;

// Namespace declarations


namespace Scene {


class ViewFrustumRenderer : public Core::IRendererPlugin
{
public:
	ViewFrustumRenderer();

public:
	// Core::IRendererPlugin implementation
	// {
	void render();
	// }

	// Connectors
	// {
	void connectFrustum(Frustum *f);
	// }

protected:

private:
	Frustum	*mFrustum;
};


}


#endif
