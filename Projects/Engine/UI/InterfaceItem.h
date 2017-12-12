
#ifndef _UI_InterfaceItem_h_
#define _UI_InterfaceItem_h_


// Library includes
//#include <string.h>

// Project includes

// Forward declarations

// Namespace declarations


class IInterfaceItem
{
public:
	virtual ~IInterfaceItem() { }

	void render() = 0;
};


class InterfaceItem : public IInterfaceItem
{
public:
	InterfaceItem();
	InterfaceItem(float left, float top, float width, float height);
	virtual ~InterfaceItem();

	void render();

protected:

private:
	float	mHeight;
	float	mLeft;
	float	mTop;
	float	mWidth;
};


#endif
