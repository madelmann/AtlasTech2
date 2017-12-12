
// Header
#include "EngineStates.h"

// Library includes

// Project includes

// Namespace declarations


namespace Core {


EngineStates::EngineStates()
: mAmbientColor(vector3f(1.f, 1.f, 1.f)),
  mCellShading(false),
  mRenderWater(false),
  mScriptSequence(false),
  //mSkyFaceCulling(),
  mWaterHeight(0.f)
{
}

void EngineStates::beginCellShading()
{
	mCellShading = true;
}

void EngineStates::beginRenderWater()
{
	mRenderWater = true;
}

void EngineStates::beginScriptSequence()
{
	mScriptSequence = true;
}

void EngineStates::endCellShading()
{
	mCellShading = false;
}

void EngineStates::endRenderWater()
{
	mRenderWater = false;
}

void EngineStates::endScriptSequence()
{
	mScriptSequence = false;
}

const vector3f& EngineStates::getAmbientColor() const
{
	return mAmbientColor;
}

GLuint EngineStates::getSkyFaceCulling() const
{
	return mSkyFaceCulling;
}

float EngineStates::getWaterHeight() const
{
	return mWaterHeight;
}

bool EngineStates::isCellShading() const
{
	return mCellShading;
}

bool EngineStates::isRenderWater() const
{
	return mRenderWater;
}

bool EngineStates::isScriptSequence() const
{
	return mScriptSequence;
}

void EngineStates::setSkyFaceCulling(GLuint culling)
{
	mSkyFaceCulling = culling;
}


}
