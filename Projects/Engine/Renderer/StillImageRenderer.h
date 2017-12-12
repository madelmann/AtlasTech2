
#ifndef _Renderer_StillImageRenderer_h_
#define _Renderer_StillImageRenderer_h_


// Library includes

// Project includes
#include <Interfaces/Common/AComponent.h>

// Forward declarations
namespace TextureSpace {
	class Texture;
}

// Namespace declarations


namespace Core {

// Forward declarations
class EngineSettings;


class StillImageRenderer : public Common::AComponent
{
public:
	StillImageRenderer();
	virtual ~StillImageRenderer();

public:
	// Common::AComponent implementation
	//{
	void configurationComplete();
	void shutdown();
	//}

	// Connectors
	// {
	void connectEngineSettings(EngineSettings *s);
	// }

	void renderImage(TextureSpace::Texture *texture, float left, float top, float width, float height);
	void renderImageAtFullScreen(TextureSpace::Texture *texture);

protected:

private:
	EngineSettings	*mEngineSettings;
};


}


#endif
