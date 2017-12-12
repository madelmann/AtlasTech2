
#ifndef Screens_MainMenu_h
#define Screens_MainMenu_h


// Library includes

// Project includes
#include <UIKit\UIScreen.h>

// Forward declarations
namespace TextureSpace {
	class IManager;
}

// Namespace declarations


class MainMenu : public UIKit::UIScreen
{
public:
	MainMenu();

public:
	void connectTextureManager(TextureSpace::IManager *m);

	void init();

protected:

private:
	TextureSpace::IManager	*mTextureManager;
};


#endif
