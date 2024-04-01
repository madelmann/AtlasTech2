
// Header
#include "UIScreen.h"

// Library includes

// Project includes
#include <Menu/MenuItem.h>

// Namespace declarations


namespace UIKit {


UIScreen::UIScreen()
{
}

UIScreen::~UIScreen()
{
	for ( MenuItems::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it ) {
		delete (*it);
	}
	mMenuItems.clear();
}

void UIScreen::addMenuItem(MenuItem *item)
{
	if ( !item ) {
		return;
	}

	mMenuItems.push_back(item);
}

bool UIScreen::onMouseClick(SDL_Event* e)
{
	float x = LOWORD(e.lParam);
	float y = HIWORD(e.lParam);

	for ( MenuItems::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it ) {
		float h = (*it)->height();
		float l = (*it)->getLeft();
		float t = (*it)->getTop();
		float w = (*it)->width();

		if ( l >= x && t >= y && l + w <= x && t + h <= y ) {
			if ( (*it)->onMouseClick(e) ) {
				return true;
			}
		}
	}

	return false;
}

bool UIScreen::onMouseDown(SDL_Event* e)
{
	float x = LOWORD(e.lParam);
	float y = HIWORD(e.lParam);

	for ( MenuItems::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it ) {
		float h = (*it)->height();
		float l = (*it)->getLeft();
		float t = (*it)->getTop();
		float w = (*it)->width();

		if ( l >= x && t >= y && l + w <= x && t + h <= y ) {
			if ( (*it)->onMouseDown(e) ) {
				return true;
			}
		}
	}

	return false;
}

bool UIScreen::onMouseMove(SDL_Event* e)
{
	float x = LOWORD(e.lParam);
	float y = HIWORD(e.lParam);

	for ( MenuItems::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it ) {
		float h = (*it)->height();
		float l = (*it)->getLeft();
		float t = (*it)->getTop();
		float w = (*it)->width();

		if ( l >= x && t >= y && l + w <= x && t + h <= y ) {
			if ( (*it)->onMouseMove(e) ) {
				return true;
			}
		}
	}

	return false;
}

bool UIScreen::onMouseUp(SDL_Event* e)
{
	float x = LOWORD(e.lParam);
	float y = HIWORD(e.lParam);

	for ( MenuItems::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it ) {
		float h = (*it)->height();
		float l = (*it)->getLeft();
		float t = (*it)->getTop();
		float w = (*it)->width();

		if ( l >= x && t >= y && l + w <= x && t + h <= y ) {
			if ( (*it)->onMouseUp(e) ) {
				return true;
			}
		}
	}

	return false;
}

void UIScreen::removeMenuItem(MenuItem *item)
{
	if ( !item ) {
		return;
	}

	mMenuItems.remove(item);
}

void UIScreen::render()
{
	aePushAttrib(GL_CULL_FACE);
	aeDisable(GL_CULL_FACE);

	aePushAttrib(GL_TEXTURE_2D);
	aeEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	aePushMatrix();
		aeLoadIdentity();

		// Pass in our 2D ortho screen coordinates like so (left, right, bottom, top).
		// The last 2 parameters are the near and far planes.
		glOrtho(mLeft, mWidth, mTop, mHeight, 0.0, 10.0);

		for ( MenuItems::const_iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it ) {
			(*it)->renderDebug();
			(*it)->render();
		}

		UIFrame::render();

	aePopMatrix();

	aePopAttrib();
	aePopAttrib();
}

void UIScreen::reshape(float left, float top, float width, float height)
{
	mLeft = left;
	mHeight = height;
	mTop = top;
	mWidth = width;
}


}
