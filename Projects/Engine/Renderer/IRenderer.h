
#ifndef _Renderer_IRenderer_h_
#define _Renderer_IRenderer_h_


// Library includes

// Project includes

// Forward declarations
namespace TextureSpace {
	class Texture;
}

// Namespace declarations


namespace Core {


class IRenderer
{
public:
	virtual ~IRenderer() { }

public:
	virtual void renderImageAtFullScreen(TextureSpace::Texture *t) = 0;
};


}


#endif
