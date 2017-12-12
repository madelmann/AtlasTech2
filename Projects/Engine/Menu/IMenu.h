
#ifndef _Menu_IMenu_h_
#define _Menu_IMenu_h_


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace MenuSpace {


class IMenu
{
public:
	virtual ~IMenu() { }

public:
	virtual void render() = 0;
};


}


#endif
