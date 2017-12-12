
#ifndef _Terrain_Plugins_RenderPlugin_h_
#define _Terrain_Plugins_RenderPlugin_h_


// Library includes

// Project includes
#include <Interfaces/Renderer/IRendererPlugin.h>

// Forward declarations

// Namespace declarations


namespace Terrain {

// Forward declarations
class Settings;

class RenderPlugin : public Core::IRendererPlugin
{
public:
	RenderPlugin();

public:
	// Core::IRendererPlugin
	// {
	void render();
	// }

	// Connectors
	// {
	void connectSettings(Settings *s);
	// }

protected:

private:
	Settings	*mSettings;
};


}


#endif
