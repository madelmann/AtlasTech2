
#ifndef _Interfaces_Renderer_IRendererPlugin_h_
#define _Interfaces_Renderer_IRendererPlugin_h_


// Library includes

// Project includes
#include <Interfaces/Core/Object.h>

// Forward declarations

// Namespace declarations


namespace Core {


class IRendererPlugin : public Object
{
public:
	virtual ~IRendererPlugin() { }

public:
	virtual void render() = 0;

protected:
	IRendererPlugin(AEid id = Core::invalidID, const std::string& name = "")
	: Object(id, name)
	{ }
};


}


#endif
