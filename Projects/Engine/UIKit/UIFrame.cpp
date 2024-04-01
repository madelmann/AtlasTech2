
// Header
#include "UIFrame.h"

// Library includes

// Project includes
#include <Texture/Texture.h>

// Namespace declarations


namespace UIKit {


UIFrame::UIFrame()
: UIBaseObject(),
  mBackground(0),
  mHeight(0.f),
  mIsContainer(true),
  mWidth(0.f)
{
}

UIFrame::~UIFrame()
{
	for ( UIFrameList::iterator it = mObjects.begin(); it != mObjects.end(); it++ ) {
		delete (*it);
	}
}

void UIFrame::add(UIFrame *frame)
{
	if ( !mIsContainer ) {
		return;
	}
	if ( !frame ) {
		return;
	}

	if ( !frame->getFont() ) {
		frame->setFont(mFont);
	}

	frame->setParent(this);
	frame->init();

	mObjects.push_back(frame);
}

float UIFrame::getHeight() const
{
	return mHeight;
}

float UIFrame::getWidth() const
{
	return mWidth;
}

void UIFrame::handleEvent(SDL_Event* e)
{
	if ( !mActive )
		return;

	for ( UIFrameList::iterator it = mObjects.begin(); it != mObjects.end(); it++ ) {
		if ( (*it)->handleEvent(e) ) {
			return;
		}
	}

	return UIBaseObject::handleEvent(e);
}

void UIFrame::remove(UIFrame *frame)
{
	if ( !frame ) {
		return;
	}

	// remove but don't destroy
	mObjects.remove(frame);
}

void UIFrame::render()
{
	float left = mLeft;
	float top = mTop;

	if ( mParent ) {
		left += mParent->getLeft();
		top += mParent->getTop();
	}

	aePushMatrix();								// Push on a new matrix so that we can just pop it off to go back to perspective mode
		aeColor(mColor);

		if ( mBackground ) {
			aeEnable(GL_TEXTURE_2D);

			mBackground->bind();
		}
		//else {
		//	aeDisable(GL_TEXTURE_2D);
		//}

		aeBegin(GL_QUADS);
			aeTexCoord2f(0.0f, 0.0f); aeVertex2f(left, top);
			aeTexCoord2f(1.0f, 0.0f); aeVertex2f(left + mWidth, top);
			aeTexCoord2f(1.0f, 1.0f); aeVertex2f(left + mWidth, top + mHeight);
			aeTexCoord2f(0.0f, 1.0f); aeVertex2f(left, top + mHeight);
		aeEnd();

		for ( UIFrameList::iterator it = mObjects.begin(); it != mObjects.end(); ++it ) {
			if ( (*it)->isVisible() ) {
				(*it)->render();
			}
		}
	aePopMatrix();
}

void UIFrame::setBackground(Texture *background)
{
	mBackground = background;
}

void UIFrame::setHeight(float height)
{
	mHeight = height;
}

void UIFrame::setWidth(float width)
{
	mWidth = width;
}


}
