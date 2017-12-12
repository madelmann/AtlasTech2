
#ifndef Light_Light_h
#define Light_Light_h


// Library includes

// Project includes
#include <Appearence/LightAttributes.h>
#include <Common/Logger.h>
#include <Driver/GLWrapper.h>
#include <Math/Vector3.h>
#include <Tools/Tools.h>
#include "Settings.h"
#include "Types.h"

// Forward declarations
class IMediaPathManager;

// Namespace declarations


namespace LightSpace {


class Light : public LightAttributes,
			  private Common::Logger
{
public:
	Light(const Common::Logger *p);
	virtual ~Light();

public:
	// Connectors
	//{
	void connectMediaPathManager(IMediaPathManager *m);
	//}

	// Providers
	//{
	Settings* provideSettings();
	//}

public:
	void clear();
	void init();
	void set();
	void unset();

	void setNoAttenuation();
	void setupAttenuation(float farAttenuationBegin, float farAttenuationEnd);
	void setupSpotExponent(float hotSpotAngle);

	/// Quick setup a directional light
	void setupDirectional(const vector4f& ambiant, const vector4f& diffuse, const vector4f& specular, const vector3f& position,
						  const vector3f& direction, float constant = 1, float linear = 0, float quadratic = 0);

	/// Quick setup a point light
	void setupPointLight(const vector4f& ambiant, const vector4f& diffuse, const vector4f& specular, const vector3f& position,
						 const vector3f& direction, float constant = 1, float linear = 0, float quadratic = 0);

	/// Quick setup a spotlight
	void setupSpotLight(const vector4f& ambiant, const vector4f& diffuse, const vector4f& specular,
						const vector3f& position, const vector3f& direction, float exponent,
						float cutoff, float constant = 1, float linear = 0, float quadratic = 0);

	bool enabled() const;
	void enabled(bool state);

	int index();
	void index(int index);

	const vector3f& position();
	void position(const vector3f& position);

protected:

private:
	aeuint		mIndex;
	Settings	mSettings;

	IMediaPathManager	*mMediaPathManager;
};


}


#endif