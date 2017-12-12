
// Header
#include "MenuItem.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Texture/Texture.h>

// Namespace declarations


namespace MenuSpace {


MenuItem::MenuItem(const std::string& name)
: Core::Id(Core::invalidID),
  Core::Name(name),
  mColorMap(0),
  mHeight(0.f),
  mWidth(0.f)
{
}

MenuItem::~MenuItem()
{
}

void MenuItem::colorMap(Texture *colormap)
{
	mColorMap = colormap;
}

float MenuItem::height() const
{
	return mHeight;
}

void MenuItem::height(float value)
{
	mHeight = value;
}

void MenuItem::render()
{
	aeColor(mColor);

	if ( mColorMap ) {
		mColorMap->bind();
	}

	float sx = mWidth * 0.5f;
	float sy = mHeight * 0.5f;

	aeBegin(GL_QUADS);
		aeTexCoord2f(0.0f, 0.0f); aeVertex2f(mLeft - sx, mTop - sy);
		aeTexCoord2f(1.0f, 0.0f); aeVertex2f(mLeft + sx, mTop - sy);
		aeTexCoord2f(1.0f, 1.0f); aeVertex2f(mLeft + sx, mTop + sy);
		aeTexCoord2f(0.0f, 1.0f); aeVertex2f(mLeft - sx, mTop + sy);
	aeEnd();

	if ( mColorMap ) {
		mColorMap->unbind();
	}
}

void MenuItem::renderDebug()
{
	aeColor3f(1.f, 1.f, 0.f);

	GLint polygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, polygonMode);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);								// Draw frontfacing polygons as wireframe

	float sx = mWidth * 0.5f;
	float sy = mHeight * 0.5f;

	aeBegin(GL_QUADS);
		aeVertex2f(mLeft - sx, mTop - sy);
		aeVertex2f(mLeft + sx, mTop - sy);
		aeVertex2f(mLeft + sx, mTop + sy);
		aeVertex2f(mLeft - sx, mTop + sy);
	aeEnd();

	glPolygonMode(GL_FRONT_AND_BACK, polygonMode[1]);									// Return to previous mode
}

float MenuItem::width() const
{
	return mWidth;
}

void MenuItem::width(float value)
{
	mWidth = value;
}


}
