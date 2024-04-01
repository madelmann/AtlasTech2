
#ifndef _UIKit_UIScreen_h_
#define _UIKit_UIScreen_h_


// Library includes
#include <list>

// Project includes
#include "UIFrame.h"

// Forward declarations
namespace MenuSpace {
	class MenuItem;
}

// Namespace declarations
using MenuSpace::MenuItem;


namespace UIKit {


class UIScreen : public UIFrame
{
public:
	UIScreen();
	virtual ~UIScreen();

public:
	virtual bool onMouseClick(SDL_Event* e);
	virtual bool onMouseDown(SDL_Event* e);
	virtual bool onMouseMove(SDL_Event* e);
	virtual bool onMouseUp(SDL_Event* e);

	virtual void render();
	virtual void reshape(float left, float top, float width, float height);

public:
	void addMenuItem(MenuItem *item);
	void removeMenuItem(MenuItem *item);

protected:

private:
	typedef std::list<MenuItem*> MenuItems;

private:
	MenuItems	mMenuItems;
};


}


#endif
