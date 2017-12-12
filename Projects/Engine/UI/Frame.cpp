
// Header
#include "Frame.h"

// Library includes

// Project includes

// Namespace declarations


namespace UI {


Frame::Frame(const char *callback)
: mActive(false),
  mCallback(callback),
  mClicked(false),
  mFocused(false),
  mLastAction(0),
  mMouseOver(false),
  mParent(0),
  mPressed(false),
  mReleased(false),
  mVisible(false),
  mWidgetType(UNKNOWN)
{
}

//void Frame::checkKeyboardEvents(KeyEvent evt, int extraInfo)
//{
//}

//void Frame::checkMouseEvents(MouseEvent &newEvent, int extraInfo, bool bits)
//{
//	GUIEventListener *eventsListener = getEventsListener();
//	bool nRelease = mPressed;
//
//	mMouseOver  = (newEvent.getY() >= windowBounds.y) &&
//				  (newEvent.getY() <= windowBounds.w) &&
//				  (newEvent.getX() >= windowBounds.x) &&
//				  (newEvent.getX() <= windowBounds.z);
//
//	if ( !mouseOver ) {
//		pressed = (extraInfo == RELEASED) ? false : pressed;
//
//		if(extraInfo == PRESSED || extraInfo == RELEASED) {
//			focused = false;
//		}
//		return;
//	}
//
//	if ( extraInfo == RELEASED && (lastAction == CLICKED || mPressed) ) {
//		mClicked = true;
//		mPressed = false;
//	}
//
//	if ( mClicked ) {
//		mFocused = true;
//	}
//
//	if ( extraInfo == CLICKED ) {
//		mPressed = true;
//	}
//
//	mLastAction = extraInfo;
//	mReleased   = !mPressed && nRelease;
//
//	if ( eventsListener && eventDetected() ) {
//		GUIEvent event = GUIEvent(this);
//		eventsListener->actionPerformed(event);
//	}
//
//	if ( !bits ) {
//		mClicked  = false;
//		mReleased = false;
//	}
//}

const void Frame::computeBounds()
{
  if ( mParent && mUpdate ) {
    const vector4f& parentBounds = mParent->getBounds();

    mPosition.z = mParent->getPosition().z + 1.f;

    vector2f newDimensions, newPosition;
 
    newDimensions.x = parentBounds.z - parentBounds.x;
    newDimensions.y = parentBounds.w - parentBounds.y;

    newPosition.x = parentBounds.x;
    newPosition.y = parentBounds.y;

    newPosition.x = (mPosition.x <= 1.0f) && (mPosition.x >= 0.0f) ? 
					newDimensions.x * mPosition.x + parentBounds.x :
					(mPosition.x < 0.0f) ?
					parentBounds.z + mPosition.x :
					mPosition.x + parentBounds.x;

    newPosition.y = (mPosition.y <= 1.0f) && (mPosition.y >= 0.0f) ? 
					newDimensions.y * mPosition.y + parentBounds.y :
					(mPosition.y < 0.0f) ?
					parentBounds.w + mPosition.y :
					mPosition.y + parentBounds.y;

    newDimensions.x  = (mDimensions.x <= 1.0f) ? newDimensions.x * mDimensions.x : mDimensions.x;
    newDimensions.y  = (mDimensions.y <= 1.0f) ? newDimensions.y * mDimensions.y : mDimensions.y;

    mBounds.x = newPosition.x;
    mBounds.y = newPosition.y;
    mBounds.z = newPosition.x + newDimensions.x;
    mBounds.w = newPosition.y + newDimensions.y;

    switch ( mAnchor ) {
		case CENTER:
			mBounds.y -= newDimensions.y / 2;
			mBounds.w -= newDimensions.y / 2;
			mBounds.x -= newDimensions.x / 2;
			mBounds.z -= newDimensions.x / 2;
			break;
		case CORNERLD:
			mBounds.y -= newDimensions.y;
			mBounds.w -= newDimensions.y;
			break;
		case CORNERRD:
			mBounds.y -= newDimensions.y;
			mBounds.w -= newDimensions.y;
			mBounds.x -= newDimensions.x;
			mBounds.z -= newDimensions.x;
			break;
		case CORNERRU:
			mBounds.x -= newDimensions.x;
			mBounds.z -= newDimensions.x;
			break;
    }
  }
}

void Frame::disableGUITexture()
{
	if ( mParent ) {
		mParent->disableGUITexture();
	}
}

void Frame::enableGUITexture()
{
	if ( mParent ) {
		mParent->enableGUITexture();
	}
}

bool Frame::eventDetected()
{
	return mClicked || mFocused || mMouseOver || mPressed || mReleased;
}

void Frame::forceUpdate(bool flag)
{
	mUpdate = flag;
}

int Frame::getAnchorPoint()
{
	return mAnchor;
}

const vector4f& Frame::getBounds()
{
	computeBounds();
	return mBounds;
}

const std::string& Frame::getCallback() const
{
	return mCallback;
}

EventListener *Frame::getEventsListener()
{
	return !mParent ? 0 : mParent->getEventsListener();
}

Frame* Frame::getParent() const
{
	return mParent;
}

const vector3f& Frame::getPosition() const
{
	return mPosition;
}

int Frame::getWidgetType() const
{ 
	return mWidgetType; 
}

bool Frame::isActive() const
{
	return mActive;
}

bool Frame::isAttached() const
{
	return ( mParent != 0 );
}

bool Frame::isClicked()
{
	return mClicked;
}

bool Frame::isFocused()
{
	return mFocused;
}

bool Frame::isMouseOver()
{
	return mMouseOver;
}

bool Frame::isPressed()
{
	return mPressed;
}

bool Frame::isReleased()
{
	return mReleased;
}

bool Frame::isVisible() const
{
	return mVisible;
}

void Frame::setActive(bool flag)
{
	mActive = flag;
}

void Frame::setAnchorPoint(const char *anchorArg)
{
	if ( !anchorArg ) {
		return;
	}

	if ( !strcmp(anchorArg, "CENTER") ) {
		mAnchor = CENTER;
	}
	else if ( !strcmp(anchorArg, "CORNERLU") ) {
		mAnchor = CORNERLU;
	}
	else if ( !strcmp(anchorArg, "CORNERRU") ) {
		mAnchor = CORNERRU;
	}
	else if ( !strcmp(anchorArg, "CORNERLD") ) {
		mAnchor = CORNERLD;
	}
	else if ( !strcmp(anchorArg, "CORNERRD") ) {
		mAnchor = CORNERRD;
	}
}

void Frame::setAnchorPoint(int anchorArg)
{
	switch ( anchorArg ) {
		case CORNERLU:
		case CORNERRU:
		case CORNERLD:
		case CORNERRD:
			mAnchor = anchorArg;
			break;
		default:
			mAnchor = CENTER;
			break;
	}

	mUpdate = true;
}

void Frame::setCallback(const std::string &callback)
{
	mCallback = callback;
}

void Frame::setParent(Frame *parent)
{
	mParent = parent;
}

void Frame::setPosition(const vector3f &pos)
{
	mPosition = pos;
}

void Frame::setVisible(bool flag)
{
	mVisible = flag;
}


}
