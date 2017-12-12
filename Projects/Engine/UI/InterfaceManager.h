
#ifndef _UI_InterfaceManager_h_
#define _UI_InterfaceManager_h_


// Library includes
#include <vector.h>

// Project includes

// Forward declarations
class InterfaceItem;

// Namespace declarations


class InterfaceManager
{
public:
	InterfaceManager();
	~InterfaceManager();

protected:

private:
	std::vector<IInterfaceItem*>	mInterfaceItems;
};


#endif
