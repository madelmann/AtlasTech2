
#ifndef _Renderer_MenuRenderer_h_
#define _Renderer_MenuRenderer_h_


// Library includes

// Project includes
#include <Interfaces\Renderer\IRendererPlugin.h>

// Forward declarations

// Namespace declarations


class MenuRenderer : public Core::IRendererPlugin
{
public:
	MenuRenderer();
	virtual ~MenuRenderer();

	virtual void render();

protected:

private:

};


#endif
