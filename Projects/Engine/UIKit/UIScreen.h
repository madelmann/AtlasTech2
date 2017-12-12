
#ifndef _UIKit_UIScreen_h_
#define _UIKit_UIScreen_h_


// Library includes
#include <list>

// Project includes
#include "UIFrame.h"
#include <Interfaces/Common/Event.h>

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
	virtual bool onMouseClick(const Common::Event& e);
	virtual bool onMouseDown(const Common::Event& e);
	virtual bool onMouseMove(const Common::Event& e);
	virtual bool onMouseUp(const Common::Event& e);

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
