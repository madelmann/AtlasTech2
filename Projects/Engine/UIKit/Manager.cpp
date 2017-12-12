
// Header
#include "Manager.h"

// Library includes

// Project includes
#include "UIScreen.h"
#include <Driver/GLWrapper.h>
#include <Tools/Strings.h>

// Namespace declarations


namespace UIKit {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "UIKit::Manager"),
  mActiveScreen(0)
{
	debug("Starting...");
}

Manager::~Manager()
{
	shutdown();
}

void Manager::add(UIScreen *screen)
{
	if ( !screen ) {
		error("invalid <Screen> provided - aborting...");
		return;
	}

	debug("add('" + screen->getName() + "')");

	if ( contains(screen) ) {
		error("duplicate screen '" + screen->getName() + "' added!");
		return;
	}

	screen->focus();

	mScreens.push_back(screen);
}

void Manager::clear()
{
	debug("Clearing...");

	// Garbage collection:
	// all screens that haven't been removed will be destroyed
	for ( ScreenList::iterator it = mScreens.begin(); it != mScreens.end(); it++ ) {
		delete (*it);
	}
	mScreens.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	mConfigurationComplete = true;
}

bool Manager::contains(UIScreen *screen)
{
	for ( ScreenList::iterator it = mScreens.begin(); it != mScreens.end(); it++ ) {
		if ( screen == (*it) ) {
			 return true;
		}
	}

	return false;
}

UIScreen* Manager::get(const std::string& name)
{
	if ( name.empty() ) {
		error("invalid screen provided - aborting...");
		return 0;
	}

	for ( ScreenList::iterator it = mScreens.begin(); it != mScreens.end(); it++ ) {
		if ( Tools::StringCompare(name, (*it)->getName()) ) {
			 return (*it);
		}
	}

	return 0;
}

LRESULT Manager::handleEvent(const Common::Event& e)
{
	if ( !mActiveScreen ) {
		return false;
	}

	return mActiveScreen->handleEvent(e);
}

void Manager::hide(const std::string& screen)
{
	if ( screen.empty() ) {
		error("invalid screen name provided - aborting...");
		return;
	}

	UIScreen *active = get(screen);
	if ( !active ) {
		return;
	}

	hide(active);
}

void Manager::hide(UIScreen *screen)
{
	if ( !screen ) {
		error("invalid <UIScreen> provided - aborting...");
		return;
	}
	if ( mActiveScreen != screen ) {
		return;
	}

	if ( mScreenStack.size() <= 1 ) {
		// don't hide our last visible screen
		screen->show();
		return;
	}

	screen->hide();

	mScreenStack.pop_back();

	UIScreen *newActive = get(mScreenStack.back());
	if ( newActive ) {
		show_no_stack(newActive->getName());
	}
}

void Manager::init()
{
	debug("Initializing...");

	mIsReady = true;
}

void Manager::manage()
{
	// nothing to do here
}

void Manager::remove(UIScreen *screen)
{
	if ( !screen ) {
		error("invalid <UIScreen> provided - aborting...");
		return;
	}

	debug("remove('" + screen->getName() + "')");

	mScreens.remove(screen);
}

void Manager::render()
{
	// only render if we're ready
	if ( !mIsReady ) {
		return;
	}

	for ( ScreenList::iterator it = mScreens.begin(); it != mScreens.end(); it++ ) {
		if ( (*it)->isVisible() ) {
			aePushMatrix();														// Push on a new matrix so that we can just pop it off to go back to perspective mode

				(*it)->render();

			aePopMatrix();
			return;																// no need to loop through all screens because only one can be visible at a time
		}
		else {
			hide((*it));
		}
	}
}

void Manager::reshape(float left, float top, float width, float height)
{
	for ( ScreenList::iterator it = mScreens.begin(); it != mScreens.end(); it++ ) {
		(*it)->reshape(left, top, width, height);
	}
}

void Manager::show(const std::string& screen)
{
	if ( screen.empty() ) {
		error("invalid screen provided - aborting...");
		return;
	}

	// Show the screen...
	show_no_stack(screen);

	// ... and push the screen to the stack
	mScreenStack.push_back(screen);
}

void Manager::show(UIScreen *screen)
{
	if ( !screen ) {
		error("invalid <UIScreen> provided - aborting...");
		return;
	}

	show(screen->getName());
}

void Manager::show_no_stack(const std::string& screen)
{
	for ( ScreenList::iterator it = mScreens.begin(); it != mScreens.end(); it++ ) {
		if ( Tools::StringCompare(screen, (*it)->getName()) ) {
			// Show the requested screen...
			mActiveScreen = (*it);
			(*it)->show();
		}
		else {
			// ... and hide all others
			(*it)->hide();
		}
	}
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::update(float elapsedTime)
{
	for ( ScreenList::iterator it = mScreens.begin(); it != mScreens.end(); it++ ) {
		(*it)->update(elapsedTime);
	}
}


}
