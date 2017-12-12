
#ifndef _Interfaces_Common_IEventDispatcher_h_
#define _Interfaces_Common_IEventDispatcher_h_


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {

// Forward declarations
class IEventHandler;

class IEventDispatcher
{
public:
	virtual ~IEventDispatcher() { }

	virtual void addEventHandler(IEventHandler *h) = 0;
	virtual void removeEventHandler(IEventHandler *h) = 0;
};


}


#endif
