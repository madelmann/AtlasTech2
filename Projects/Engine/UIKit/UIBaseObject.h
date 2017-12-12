
#ifndef _UIKit_UIBaseObject_h_
#define _UIKit_UIBaseObject_h_


// Library includes
#include <string>

// Project includes
#include "Types.h"
#include <Driver/GLWrapper.h>
#include <Font/Font.h>
#include <Interfaces/Common/Event.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations
using FontSpace::Font;


namespace UIKit {


// This is an abstract class which is the base for all ui object classes

class UIBaseObject
{
public:
	UIBaseObject();
	virtual ~UIBaseObject();

public:
	virtual void init();
	virtual void update(float elapsedTime = 0.f);

	virtual void render() = 0;

public:	// Events
	void setMouseClickEvent(void (*e)(const Common::Event& e));
	void setMouseDownEvent(FPtr);
	void setMouseMoveEvent(FPtr);
	void setMouseUpEvent(FPtr);

	virtual LRESULT handleEvent(const Common::Event& e);

	virtual bool onKeyDown(WPARAM /*key*/) { return false; }
	virtual bool onKeyPress(WPARAM /*key*/) { return false; }
	virtual bool onKeyUp(WPARAM /*key*/) { return false; }

	virtual bool onMouseClick(const Common::Event& /*e*/) { return false; }
	virtual bool onMouseDblClick(const Common::Event& /*e*/) { return false; }
	virtual bool onMouseDown(const Common::Event&  /*e*/) { return false; }
	virtual bool onMouseMove(const Common::Event&  /*e*/) { return false; }
	virtual bool onMouseUp(const Common::Event&  /*e*/) { return false; }

public:
	void focus();
	void hide();
	void show();

	bool isActive() const;
	bool isVisible() const;

	void setActive(bool flag);

	const vector3f& getColor() const;
	void setColor(const vector3f& color);

	Font* getFont();
	void setFont(Font *font);

	float getLeft() const;
	void setLeft(float left);

	const std::string& getName() const;
	void setName(const std::string& name);

	UIBaseObject* getParent();
	void setParent(UIBaseObject *parent);

	const std::string& getText() const;
	void setText(const std::string& text);

	float getTop() const;
	void setTop(float top);

protected:
	bool			mActive;
	vector3f		mColor;
	bool			mFocus;
	Font			*mFont;
	bool			mIsInitilized;
	float			mLeft;
	std::string		mName;
	UIBaseObject	*mParent;
	std::string		mText;
	float			mTop;
	bool			mVisible;

protected:	// Events
	void (*MouseClick)(const Common::Event& e);
	FPtr	MouseDown;
	FPtr	MouseMove;
	FPtr	MouseUp;


private:

};


}


#endif
