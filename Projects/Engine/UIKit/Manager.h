
#ifndef UIKit_Manager_h
#define UIKit_Manager_h


// Library includes
#include <list>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/UIKit/IManager.h>

// Forward declarations

// Namespace declarations


namespace UIKit {

// Forward declarations

class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

public:
	// Common::AManager implementation
	// {
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime = 0.f);
	// }

	// UIKit::IManager implementation
	// {
	void add(UIScreen *screen);
	bool contains(UIScreen *screen);
	UIScreen* get(const std::string& name);
	LRESULT handleEvent(SDL_Event* e);
	void hide(UIScreen *screen);
	void hide(const std::string& screen);
	void remove(UIScreen *screen);
	void render();
	void reshape(float left, float top, float width, float height);
	void show(const std::string& screen);
	void show(UIScreen *screen);
	// }

protected:

private:
	typedef std::list<UIScreen*> ScreenList;
	typedef std::list<std::string> ScreenStack;

private:
	void show_no_stack(const std::string& screen);

private:
	UIScreen	*mActiveScreen;
	ScreenList	mScreens;
	ScreenStack	mScreenStack;
};


}


#endif
