
#ifndef _Driver_DriverExtensions_h_
#define _Driver_DriverExtensions_h_


// Library includes

// Project includes
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations


class DriverExtensions
{
public:
	struct TextureCompression_t {
		AEint*	mFormats;
		AEint	mNumFormats;
	};

public:
	DriverExtensions()
	: mAnsitropicFilter(1.f),
	  mFrameBufferEXT(false),
	  mMaxLights(0),
	  mOcclusionQuery(false),
	  mGL_ARB_point_parameters(false),
	  mglActiveTextureARB(false),
	  mglFogCoordfEXT(false),
	  mVertexBufferObjects(false)
	{ }

public:
	AEfloat	mAnsitropicFilter;
	bool	mFrameBufferEXT;
	AEint	mMaxLights;
	bool	mOcclusionQuery;
	bool	mVertexBufferObjects;

	bool					mGL_ARB_point_parameters;
	bool					mglActiveTextureARB;
	bool					mglFogCoordfEXT;
	bool					mglMultiTexCoord2fARB;
	bool					mglShaders;
	TextureCompression_t	mTextureCompression;
};


#endif
