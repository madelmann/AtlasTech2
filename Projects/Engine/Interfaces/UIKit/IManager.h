
#ifndef Interfaces_UIKit_IManager_h
#define Interfaces_UIKit_IManager_h


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/AManager.h>
#include <Interfaces/Common/Event.h>

// Forward declarations

// Namespace declarations


namespace UIKit {

// Forward declarations
class UIScreen;

class IManager : public Common::AManager
{
public:
	virtual ~IManager() { }

public:
	virtual void add(UIScreen *screen) = 0;
	virtual bool contains(UIScreen *screen) = 0;
	virtual UIScreen* get(const std::string& screen) = 0;
	virtual LRESULT handleEvent(const Common::Event& e) = 0;
	virtual void hide(const std::string& screen) = 0;
	virtual void hide(UIScreen *screen) = 0;
	virtual void remove(UIScreen *screen) = 0;
	virtual void render() = 0;
	virtual void reshape(float left, float top, float width, float height) = 0;
	virtual void show(const std::string& screen) = 0;
	virtual void show(UIScreen *screen) = 0;
};


}


#endif
