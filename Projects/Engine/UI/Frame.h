
#ifndef _UI_Frame_h_
#define _UI_Frame_h_


// Library includes
#include <string>

// Project includes
#include "EventListener.h"
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

// Forward declarations

// Namespace declarations


namespace UI {


enum AnchorPoints {
  CENTER,
  CORNERLU,
  CORNERRU,
  CORNERLD,
  CORNERRD
};

enum WidgetEvents {
  CLICKED = 1,
  DOUBLE_CLICKED,
  PRESSED,
  DRAGGED,
  RELEASED,
  MOVED
};

enum WidgetTypes {
  SLIDER    = 1,
  BUTTON,
  CHECK_BOX,
  CHECK_BOX_MARK,
  RADIO_BUTTON,
  CHECK_RB_MARK,
  LABEL,
  TEXT_AREA,
  MATERIAL_SURFACE,
  PANEL,
  SEPARATOR,
  TEXT_BOX,
  COMBO_BOX,
  TABBED_PANEL,
  UNKNOWN
};


class Frame
{
public:
	Frame(const char *callback = 0);

    bool isAttached() const;

	bool isActive() const;
	void setActive(bool flag);

    bool isVisible() const;
    void setVisible(bool flag);

	const std::string& getCallback() const;
	void setCallback(const std::string &callback);

	const vector3f& getPosition() const;
	void setPosition(const vector3f &pos);

	int getWidgetType() const;

    void setAnchorPoint(const char *anchor);
    void setAnchorPoint(int anchor);
    int getAnchorPoint();

    virtual void forceUpdate(bool flag);

    //virtual void checkMouseEvents(MouseEvent  &evt, int extraInfo, bool reservedBits = false);
    //virtual void checkKeyboardEvents(KeyEvent  evt, int extraInfo);
    virtual void render(float) = 0;

    virtual Frame* getParent() const;
    virtual void setParent(Frame *parent);

    //virtual  GUITexCoordDescriptor *getTexCoordsInfo(int type);
    virtual EventListener *getEventsListener();

    virtual void disableGUITexture();
    virtual void enableGUITexture();

    virtual const void computeBounds();
    virtual const vector4f& getBounds();

    bool eventDetected();
    bool isMouseOver();
    bool isReleased();
    bool isFocused();
    bool isDragged();
    bool isClicked();
    bool isPressed();

protected:
	bool		mActive;
	vector4f	mBounds;
	std::string	mCallback;
	vector2f	mDimensions;
	Frame		*mParent;
	vector3f	mPosition;
	bool		mVisible;

	bool	mClicked;
	bool	mFocused;
	bool	mMouseOver;
	bool	mPressed;
	bool	mReleased;
	bool	mUpdate;

	int		mAnchor;
	int		mLastAction;
	int		mWidgetType;

private:

};


}


#endif
