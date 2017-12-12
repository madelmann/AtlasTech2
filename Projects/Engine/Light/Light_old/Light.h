
#ifndef _LIGHT_H
#define _LIGHT_H


// Library includes

// Project includes
#include <Math\Vector4.h>

// Forward declarations

// Namespace declarations


class Light
{
public:
	Light()
	{
		bEnabled				= false;
		fAttenuationConstant	= 1.0f;
		fAttenuationLinear		= 0.0f;
		fAttenuationQuadratic	= 0.0f;
		fSpotExponent			= 0.0f;
		fSpotCutOff				= 180.0f;
		fvAmbientColor[0]		= 0.0f;
		fvAmbientColor[1]		= 0.0f;
		fvAmbientColor[2]		= 0.0f;
		fvAmbientColor[3]		= 1.0f;
		fvDiffuseColor[0]		= 1.0f;
		fvDiffuseColor[1]		= 1.0f;
		fvDiffuseColor[2]		= 1.0f;
		fvDiffuseColor[3]		= 1.0f;
		fvSpecularColor[0]		= 1.0f;
		fvSpecularColor[1]		= 1.0f;
		fvSpecularColor[2]		= 1.0f;
		fvSpecularColor[3]		= 1.0f;
		fvPosition[0]			= 0.0f;
		fvPosition[1]			= 1.0f;
		fvPosition[2]			= 0.0f;
		fvPosition[3]			= 0.0f;
		fvSpotDirection[0]		= 0.0f;
		fvSpotDirection[1]		= 0.0f;
		fvSpotDirection[2]		= -1.0f;
	};
	void clear()
	{
		bEnabled				= false;
		fAttenuationConstant	= 1.0f;
		fAttenuationLinear		= 0.0f;
		fAttenuationQuadratic	= 0.0f;
		fSpotExponent			= 0.0f;
		fSpotCutOff				= 180.0f;
		fvAmbientColor[0]		= 0.0f;
		fvAmbientColor[1]		= 0.0f;
		fvAmbientColor[2]		= 0.0f;
		fvAmbientColor[3]		= 1.0f;
		fvDiffuseColor[0]		= 1.0f;
		fvDiffuseColor[1]		= 1.0f;
		fvDiffuseColor[2]		= 1.0f;
		fvDiffuseColor[3]		= 1.0f;
		fvSpecularColor[0]		= 1.0f;
		fvSpecularColor[1]		= 1.0f;
		fvSpecularColor[2]		= 1.0f;
		fvSpecularColor[3]		= 1.0f;
		fvPosition[0]			= 0.0f;
		fvPosition[1]			= 1.0f;
		fvPosition[2]			= 0.0f;
		fvPosition[3]			= 0.0f;
		fvSpotDirection[0]		= 0.0f;
		fvSpotDirection[1]		= 0.0f;
		fvSpotDirection[2]		= -1.0f;
	};
	void disable()
	{
		bEnabled = false;
		aeDisable(GL_LIGHT0 + iIndex);
	};
	void enable()
	{
		bEnabled = true;
		aeEnable(GL_LIGHT0 + iIndex);

		setLight();
	};
	void setAmbient(const vector4f& ambient) {
		fvAmbientColor[0] = ambient.x;
		fvAmbientColor[1] = ambient.y;
		fvAmbientColor[2] = ambient.z;
		fvAmbientColor[3] = ambient.w;
	}
	void setDiffuse(const vector4f& diffuse) {
		fvDiffuseColor[0] = diffuse.x;
		fvDiffuseColor[1] = diffuse.y;
		fvDiffuseColor[2] = diffuse.z;
		fvDiffuseColor[3] = diffuse.w;
	}
	void setIndex(GLuint index) {
		iIndex = index;
	}
	void setLight()
	{
		glLightfv(GL_LIGHT0 + iIndex, GL_AMBIENT,				fvAmbientColor);
		glLightfv(GL_LIGHT0 + iIndex, GL_DIFFUSE,				fvDiffuseColor);
		glLightfv(GL_LIGHT0 + iIndex, GL_SPECULAR,				fvSpecularColor);
		glLightfv(GL_LIGHT0 + iIndex, GL_POSITION,				fvPosition);
		glLightfv(GL_LIGHT0 + iIndex, GL_SPOT_DIRECTION,		fvSpotDirection);
		glLightfv(GL_LIGHT0 + iIndex, GL_SPOT_EXPONENT,			&fSpotExponent);
		glLightfv(GL_LIGHT0 + iIndex, GL_CONSTANT_ATTENUATION,	&fAttenuationConstant);
		glLightfv(GL_LIGHT0 + iIndex, GL_LINEAR_ATTENUATION,	&fAttenuationLinear);
		glLightfv(GL_LIGHT0 + iIndex, GL_QUADRATIC_ATTENUATION,	&fAttenuationQuadratic);
	};
	void setPosition(const vector4f& pos) {
		fvPosition[0] = pos.x;
		fvPosition[1] = pos.y;
		fvPosition[2] = pos.z;
		fvPosition[3] = pos.w;
	}
	void setSpecular(const vector4f& specular) {
		fvSpecularColor[0] = specular.x;
		fvSpecularColor[1] = specular.y;
		fvSpecularColor[2] = specular.z;
		fvSpecularColor[3] = specular.w;
	}
	/* Spot Light Functions */
	void setSpotDirection(GLfloat fv[3])
	{
		fvSpotDirection[0] = fv[0];
		fvSpotDirection[1] = fv[1];
		fvSpotDirection[2] = fv[2];
	};
	void getSpotDirection(GLfloat* fv[3])
	{
		*fv[0] = fvSpotDirection[0];
		*fv[1] = fvSpotDirection[1];
		*fv[2] = fvSpotDirection[2];
	};
	void setSpotExponent(float exponent)
	{
		fSpotExponent = exponent;
	};
	float getSpotExponent()
	{
		return fSpotExponent;
	};
	void setSpotCutoff(float cutoff)
	{
		fSpotCutOff = cutoff;
	};
	float getSpotCutoff()
	{
		return fSpotCutOff;
	};
	/* Attenuation factors */
	void setAttenuationConstant(float constant)
	{
		fAttenuationConstant = constant;
	};
	float getAttenuationConstant()
	{
		return fAttenuationConstant;
	};
	void setAttenuationLinear(float linear)
	{
		fAttenuationLinear = linear;
	};
	float getAttenuationLinear()
	{
		return fAttenuationLinear;
	};
	void setAttenuationQuadratic(float quadratic)
	{
		fAttenuationQuadratic = quadratic;
	};
	float getAttenuationQuadratic()
	{
		return fAttenuationQuadratic;
	};

	bool	bEnabled;
	float	fAttenuationConstant;
	float	fAttenuationLinear;
	float	fAttenuationQuadratic;
	float	fSpotCutOff;
	float	fSpotExponent;
	GLfloat	fvAmbientColor[4];
	GLfloat	fvDiffuseColor[4];
	GLfloat	fvSpecularColor[4];
	GLfloat	fvSpotDirection[3];
	GLfloat	fvPosition[4];
	int		iIndex;
	GLenum	uType;
};

#endif
