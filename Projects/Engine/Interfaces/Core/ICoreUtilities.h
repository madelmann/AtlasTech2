
#ifndef _Interfaces_Core_ICoreUtilitie_h_
#define _Interfaces_Core_ICoreUtilitie_h_


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Core {

// Forward declarations
class IRendererPlugin;

class ICoreUtilities
{
public:
	virtual ~ICoreUtilities() { }

public:
	virtual void registerPlugin(IRendererPlugin *plugin) = 0;
	virtual void unregisterPlugin(IRendererPlugin *plugin) = 0;
};


}

#endif
