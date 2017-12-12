
// Header
#include "EngineSettings.h"

// Library includes

// Project includes
#include <Driver/DriverExtensions.h>

// Namespace declarations


namespace Core {


EngineSettings::EngineSettings()
: mAnsitropicFilter(1.f),
  mColorMaterial(false),
  mCompressedTextures(false),
  mEditorMode(false),
  mFogHint(GL::Hint::gl_dont_care),
  mFOV(50),
  mFragmentShaderDerivativeHint(GL::Hint::gl_dont_care),
  mFullScreen(false),
  mGenerateMipmapHint(GL::Hint::gl_dont_care),
  mIsConsoleVisible(false),
  mLightModelTwoSided(false),
  mLineSmoothHint(GL::Hint::gl_dont_care),
  mMaximumFramerate(60.f),
  mMaxLights(8),
  mMaxLODLevel(1),
  mMaxOcclusionCalculatedBeforeFPS(1),
  mMaxViewDepth(0.f),
  mPauseWhileInactive(true),
  mPerspectiveCorrectionHint(GL::Hint::gl_dont_care),
  mPointSmoothHint(GL::Hint::gl_dont_care),
  mPolygonSmoothHint(GL::Hint::gl_dont_care),
  mTextureCompressionHint(GL::Hint::gl_dont_care),
  mTimeBetweenOcclusionQueries(0.f),
  mUseDetailTextures(false),
  mUseFlatShading(false),
  mUseFog(false),
  mUseHighPerformanceCounter(false),
  mUseMultiTexturing(false),
  mUseOcclusionQuery(false),
  mUseShaders(false),
  mUseTextures(false)
{
}

GL::Hint::E EngineSettings::getFogHint() const
{
	return mFogHint;
}

GL::Hint::E EngineSettings::getFragmentShaderDerivativeHint() const
{
	return mFragmentShaderDerivativeHint;
}

GL::Hint::E EngineSettings::getGenerateMipmapHint() const
{
	return mGenerateMipmapHint;
}

GL::Hint::E EngineSettings::getLineSmoothHint() const
{
	return mLineSmoothHint;
}

GL::Hint::E EngineSettings::getPerspectiveCorrectionHint() const
{
	return mPerspectiveCorrectionHint;
}

GL::Hint::E EngineSettings::getPointSmoothHint() const
{
	return mPointSmoothHint;
}

GL::Hint::E EngineSettings::getPolygonSmoothHint() const
{
	return mPolygonSmoothHint;
}

GL::Hint::E EngineSettings::getTextureCompressionHint() const
{
	return mTextureCompressionHint;
}

void EngineSettings::reset()
{
	mAnsitropicFilter = 1.f;
	mColorMaterial = false;
	mCompressedTextures = false;
	mFOV = 50.f;
	mLightModelTwoSided = false;
	mMaxLODLevel = 1;
	mMaxOcclusionCalculatedBeforeFPS = 1;
}

void EngineSettings::setFogHint(GL::Hint::E hint)
{
	mFogHint = hint;
}

void EngineSettings::setFragmentShaderDerivativeHint(GL::Hint::E hint)
{
	mFragmentShaderDerivativeHint = hint;
}

void EngineSettings::setGenerateMipmapHint(GL::Hint::E hint)
{
	mGenerateMipmapHint = hint;
}

void EngineSettings::setLineSmoothHint(GL::Hint::E hint)
{
	mLineSmoothHint = hint;
}

void EngineSettings::setPerspectiveCorrectionHint(GL::Hint::E hint)
{
	mPerspectiveCorrectionHint = hint;
}

void EngineSettings::setPointSmoothHint(GL::Hint::E hint)
{
	mPointSmoothHint = hint;
}

void EngineSettings::setPolygonSoothHint(GL::Hint::E hint)
{
	mPolygonSmoothHint = hint;
}

void EngineSettings::setTextureCompressionHint(GL::Hint::E hint)
{
	mTextureCompressionHint = hint;
}


}
