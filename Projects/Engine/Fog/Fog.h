
#ifndef Fog_Fog_h
#define Fog_Fog_h


// Library includes

// Project includes
#include <Interfaces/Appearence/IAttribute.h>

// Forward declarations

// Namespace declarations


namespace FogSpace {

// Forward declarations
class Settings;

class Fog : public IAttribute
{
public:
	Fog(Settings *s);

public:
	void bind();
	void unbind();

protected:

private:
	Settings	*mSettings;
};


}


#endif
