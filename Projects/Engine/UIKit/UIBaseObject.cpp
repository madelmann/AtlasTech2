
// Header
#include "UIBaseObject.h"

// Library includes
#include <cassert>

// Project includes

// Namespace declarations


namespace UIKit {


UIBaseObject::UIBaseObject()
: mActive(false),
  mColor(1.f, 1.f, 1.f),
  mFocus(false),
  mFont(0),
  mIsInitilized(false),
  mLeft(0.f),
  MouseClick(0),
  MouseDown(0),
  MouseMove(0),
  MouseUp(0),
  mParent(0),
  mTop(0.f),
  mVisible(false)
{
}

UIBaseObject::~UIBaseObject()
{
}

void UIBaseObject::focus()
{
	mActive = true;
}

const vector3f& UIBaseObject::getColor() const
{
	return mColor;
}

Font* UIBaseObject::getFont()
{
	return mFont;
}

float UIBaseObject::getLeft() const
{
	return mLeft;
}

const std::string& UIBaseObject::getName() const
{
	return mName;
}

UIBaseObject* UIBaseObject::getParent()
{
	return mParent;
}

const std::string& UIBaseObject::getText() const
{
	return mText;
}

float UIBaseObject::getTop() const
{
	return mTop;
}

LRESULT UIBaseObject::handleEvent(const Common::Event& e)
{
	if ( !mActive )
		return false;

	switch ( e.uMsg ) {
		case WM_KEYDOWN:
			return onKeyDown(e.wParam);
		case WM_KEYUP:
			return onKeyUp(e.wParam);
		case WM_MOUSEMOVE:
			return onMouseMove(e);
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			return onMouseDblClick(e);
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			return onMouseDown(e);
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			return onMouseUp(e);
	}

	return false;
}

void UIBaseObject::hide()
{
	mActive = false;
	mVisible = false;
}

void UIBaseObject::init()
{
	assert(mFont);

	mIsInitilized = true;
}

bool UIBaseObject::isActive() const
{
	return mActive;
}

bool UIBaseObject::isVisible() const
{
	return mVisible;
}

void UIBaseObject::setActive(bool flag)
{
	if ( !mVisible ) {
		return;
	}

	mActive = flag;
}

void UIBaseObject::setColor(const vector3f& color)
{
	mColor = color;
}

void UIBaseObject::setFont(Font *font)
{
	mFont = font;
}

void UIBaseObject::setLeft(float left)
{
	mLeft = left;
}

void UIBaseObject::setMouseClickEvent(void (*e)(const Common::Event& e))
{
	MouseClick = e;
}

void UIBaseObject::setMouseDownEvent(void (*e)(const Common::Event& e))
{
	MouseDown = e;
}

void UIBaseObject::setMouseMoveEvent(void (*e)(const Common::Event& e))
{
	MouseMove = e;
}

void UIBaseObject::setMouseUpEvent(void (*e)(const Common::Event& e))
{
	MouseUp = e;
}

void UIBaseObject::setName(const std::string& name)
{
	mName = name;
}

void UIBaseObject::setParent(UIBaseObject *parent)
{
	mParent = parent;
}

void UIBaseObject::setText(const std::string &text)
{
	mText = text;
}

void UIBaseObject::setTop(float top)
{
	mTop = top;
}

void UIBaseObject::show()
{
	mActive = true;
	mVisible = true;
}

void UIBaseObject::update(float /*elapsedTime*/)
{
	// nothing to do here
}


}
