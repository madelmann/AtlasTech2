
#ifndef _Interfaces_Common_IEventHandler_h_
#define _Interfaces_Common_IEventHandler_h_


// Library includes

// Project includes
#include "Event.h"

// Forward declarations

// Namespace declarations


namespace Common {


class IEventHandler
{
public:
	virtual ~IEventHandler() { }

	virtual LRESULT handleEvent(const Event& e) = 0;

protected:

private:

};


}

#endif
