
// Header
#include "FrameSettings.h"

// Library includes

// Project includes

// Namespace declarations


namespace Renderer {


FrameSettings::FrameSettings()
: mViewWidth(0.f)
{
}

const vector3f& FrameSettings::color() const
{
	return mColor;
}

void FrameSettings::color(const vector3f& color)
{
	mColor = color;
}

float FrameSettings::viewWidth() const
{
	return mViewWidth;
}

void FrameSettings::viewWidth(float viewWidth)
{
	mViewWidth = viewWidth;
}


}
