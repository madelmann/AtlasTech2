
#ifndef _Renderer_HUDRenderer_h_
#define _Renderer_HUDRenderer_h_


// Library includes

// Project includes
#include <Interfaces\Renderer\IRendererPlugin.h>

// Forward declarations

// Namespace declarations


class HUDRenderer : public Core::IRendererPlugin
{
public:
	HUDRenderer();
	virtual ~HUDRenderer();

	virtual void render();

protected:

private:
	void renderHUD();
};


#endif
