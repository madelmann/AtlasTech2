
// Header
#include "Light.h"

// Library includes

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Math/Tools.h>
#include <Parsers/LineParser.h>
#include <Tools/Tools.h>

// Namespace declarations
using namespace Tools;


namespace LightSpace {


Light::Light(const Common::Logger *p)
: Common::Logger(p, "Light"),
  mIndex(0),
  mMediaPathManager(0)
{
	debug("Starting...");

	clear();
}

Light::~Light()
{
	debug("Stopping...");

	clear();
}

void Light::clear()
{
	debug("Clearing...");

	mSettings.clear();

	mIndex = 0;
}

void Light::connectMediaPathManager(IMediaPathManager *m)
{
	assert(m);
	assert(!mMediaPathManager);

	mMediaPathManager = m;
}

bool Light::enabled() const
{
	return mSettings.enabled();
}

void Light::enabled(bool state)
{
	mSettings.enabled(state);
}

int Light::index()
{
	return mIndex;
}

void Light::index(int index)
{
	mIndex = index;
}

void Light::init()
{
	debug("Initializing...");

	assert(mMediaPathManager);

	std::string file = mMediaPathManager->buildConfigPath("light.conf");

	Core::Configuration::Configurator config(this, file);
	if ( !config.success() ) {
		warn("could not load configuration from file '" + file  + "'!");
		return;
	}

	mSettings.ambient(config.getValue("ambient").toVector4f());
	mSettings.attenuation(config.getValue("attenuation").toVector3f());
	mSettings.constantAttenuation(config.getValue("attenuation_constant").toFloat());
	mSettings.diffuse(config.getValue("diffuse").toVector4f());
	mSettings.direction(config.getValue("direction").toVector3f());
	mSettings.enabled(config.getValue("enabled").toBool());
	mSettings.lightModel().globalAmbient(config.getValue("lightmodel.globalambient").toVector4f());
	mSettings.lightModel().localViewer(config.getValue("lightmodel.localviewer").toBool());
	mSettings.lightModel().twoSided(config.getValue("lightmodel.twosided").toBool());
	mSettings.linearAttenuation(config.getValue("attenuation_linear").toFloat());
	mSettings.mode(LightType::convert(config.getValue("mode").toInt()));
	mSettings.position(config.getValue("position").toVector3f());
	mSettings.quadraticAttenuation(config.getValue("attenuation_quadratic").toFloat());
	mSettings.specular(config.getValue("specular").toVector4f());
}

const vector3f& Light::position()
{
	return mSettings.position();
}

void Light::position(const vector3f& position)
{
	mSettings.position(position);
}

Settings* Light::provideSettings()
{
	return &mSettings;
}

void Light::set()
{
	// Minimize state changes
	if ( !mSettings.enabled() ) {
		if ( mSettings.hasChanged() ) {
			aeDisable(GL_LIGHTING);
		}

		return;
	}
	else {
		if ( mSettings.hasChanged() ) {
			aeEnable(GL_LIGHTING);
		}
	}

	aeuint lightNum = GL_LIGHT0 + mIndex;

	vector3f attenuation = mSettings.attenuation();
	glLightfv(lightNum, GL_CONSTANT_ATTENUATION,	&attenuation.x);
	glLightfv(lightNum, GL_LINEAR_ATTENUATION,		&attenuation.y);
	glLightfv(lightNum, GL_QUADRATIC_ATTENUATION,	&attenuation.z);

	GLfloat tmp4[4];

	vector4f ambient = mSettings.ambient();
	tmp4[0] = ambient.x;
	tmp4[1] = ambient.y;
	tmp4[2] = ambient.z;
	tmp4[3] = ambient.z;
	glLightfv(lightNum, GL_AMBIENT, tmp4);

	vector4f diffuse = mSettings.diffuse();
	tmp4[0] = diffuse.x;
	tmp4[1] = diffuse.y;
	tmp4[2] = diffuse.z;
	tmp4[3] = diffuse.z;
	glLightfv(lightNum, GL_DIFFUSE, tmp4);

	vector4f specular = mSettings.specular();
	tmp4[0] = specular.x;
	tmp4[1] = specular.y;
	tmp4[2] = specular.z;
	tmp4[3] = specular.z;
	glLightfv(lightNum, GL_SPECULAR, tmp4);

	LightType::E type = mSettings.mode();

	vector3f pos = mSettings.position();
	tmp4[0] = pos.x;
	tmp4[1] = pos.y;
	tmp4[2] = pos.z;
	tmp4[3] = 1.f;
	if ( type != LightType::DirectionalLight ) {
		tmp4[3] = 0.f;
	}
	glLightfv(lightNum, GL_POSITION, tmp4);

	if ( type == LightType::SpotLight ) {
		// Get the cutoff of the spot
		float cutoff = 180.f * (mSettings.cutOff() / PI);
		glLightfv(lightNum, GL_SPOT_CUTOFF, &cutoff);

		vector3f dir = mSettings.direction();
		tmp4[0] = dir.x;
		tmp4[1] = dir.y;
		tmp4[2] = dir.z;
		glLightfv(lightNum, GL_SPOT_DIRECTION, tmp4);

		float exponent = mSettings.exponent();
		glLightfv(lightNum, GL_SPOT_EXPONENT, &exponent);
	}
	else {
		// Deactivate spot properties
		glLighti(lightNum, GL_SPOT_CUTOFF, 180);
		glLighti(lightNum, GL_SPOT_EXPONENT, 0);
	}

	if ( mSettings.lightModel().hasChanged() ) {
		tmp4[0] = mSettings.lightModel().globalAmbient().x;
		tmp4[1] = mSettings.lightModel().globalAmbient().y;
		tmp4[2] = mSettings.lightModel().globalAmbient().z;
		tmp4[3] = mSettings.lightModel().globalAmbient().w;
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, tmp4);
		//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, mSettings.lightModel().localViewer());
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, mSettings.lightModel().twoSided());
	}

	if ( mSettings.enabled() ) {
		aeEnable(GL_LIGHT0 + mIndex);
	}
	else {
		aeDisable(GL_LIGHT0 + mIndex);
	}
}

void Light::setNoAttenuation()
{
	mSettings.constantAttenuation(1.f);
	mSettings.linearAttenuation(0.f);
	mSettings.quadraticAttenuation(0.f);
}

void Light::setupAttenuation(float farAttenuationBegin, float farAttenuationEnd)
{
	// limit case
	if ( farAttenuationEnd <= 0 ) {
		mSettings.constantAttenuation(1000000.f);
		mSettings.linearAttenuation(0.f);
		mSettings.quadraticAttenuation(0.f);
	}
	else {
		// The following factors are "it feels good for farAttenuationBegin=0/farAttenuationEnd=1" factors.
		// btw, at r=farAttenuationEnd=1, att= 1/11 ~= 0.
		const float	constant = 1.0f;
		const float	linear = 0.f;
		const float	quadratic = 10.0f;

		/*
			With GL 'att=1/(c+l*r+q*r2)' formula, it is impossible to simulate correctly
			farAttenuationBegin (very big decrase if for instance farAttenuationBegin is near farAttenuationEnd),
			hence simulate it very badly by multiplying the farAttenuationEnd by some factor
		*/
		float	factor = 1.f;
		if ( farAttenuationBegin / farAttenuationEnd > 0.5f ) {
			factor = 2.f;
		}
		else if ( farAttenuationBegin > 0 ) {
			factor = 1.f + 2 * farAttenuationBegin / farAttenuationEnd;
		}
		farAttenuationEnd *= factor;

		// scale according to farAttenuationEnd.
		mSettings.constantAttenuation(constant);
		mSettings.linearAttenuation(linear / farAttenuationEnd);
		mSettings.quadraticAttenuation(quadratic / SQR(farAttenuationEnd));
	}
}

void Light::setupSpotExponent(float hotSpotAngle)
{
	float divid = log(cos(hotSpotAngle));

	if ( divid == 0.f) {
		divid=0.0001f;
	}

	mSettings.exponent(log(0.9f) / divid);
}

/// Quick setup a directional light
void Light::setupDirectional (const vector4f& ambient, const vector4f& diffuse, const vector4f& specular, const vector3f& position,
							   const vector3f& direction, float constant, float linear, float quadratic)
{
	mSettings.enabled(true);

	// Set the mode
	mSettings.mode(LightType::DirectionalLight);

	// Set the colors
	mSettings.ambient(ambient);
	mSettings.diffuse(diffuse);
	mSettings.specular(specular);

	// Set the direction
	mSettings.direction(direction);

	// Set attenuation
	mSettings.constantAttenuation(constant);
	mSettings.linearAttenuation(linear);
	mSettings.quadraticAttenuation(quadratic);

	// Dummy to avoid uninit data, and problems of cache
	mSettings.position(position);
	mSettings.exponent(0.f);
	mSettings.cutOff(0.f);

	// Set it
	set();
}

/// Quick setup a point light
void Light::setupPointLight (const vector4f& ambient, const vector4f& diffuse, const vector4f& specular, const vector3f& position,
							  const vector3f& direction, float constant, float linear, float quadratic)
{
	mSettings.enabled(true);

	// Set the mode
	mSettings.mode(LightType::DirectionalLight);

	// Set the colors
	mSettings.ambient(ambient);
	mSettings.diffuse(diffuse);
	mSettings.specular(specular);

	// Set the direction
	mSettings.direction(direction);

	// Set attenuation
	mSettings.constantAttenuation(constant);
	mSettings.linearAttenuation(linear);
	mSettings.quadraticAttenuation(quadratic);

	// Dummy to avoid uninit data, and problems of cache
	mSettings.position(position);
	mSettings.exponent(0.f);
	mSettings.cutOff(0.f);

	// Set it
	set();
}

/// Quick setup a spotlight
void Light::setupSpotLight (const vector4f& ambient, const vector4f& diffuse, const vector4f& specular,
							 const vector3f& position, const vector3f& direction, float exponent,
							 float cutoff, float constant, float linear, float quadratic)
{
	mSettings.enabled(true);

	// Set the mode
	mSettings.mode(LightType::SpotLight);

	// Set the colors
	mSettings.ambient(ambient);
	mSettings.diffuse(diffuse);
	mSettings.specular(specular);

	// Set the position and direction
	mSettings.position(position);
	mSettings.direction(direction);

	// Set spotlight parameters
	mSettings.exponent(exponent);
	mSettings.cutOff(cutoff);

	// Set attenuation
	mSettings.constantAttenuation(constant);
	mSettings.linearAttenuation(linear);
	mSettings.quadraticAttenuation(quadratic);

	// Set it
	set();
}

void Light::unset()
{
	aeDisable(GL_LIGHTING);
}


}
