
#ifndef _Interfaces_Common_IEventHandler_h_
#define _Interfaces_Common_IEventHandler_h_


// Library includes
#include <SDL2/SDL_events.h>

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {


class IEventHandler
{
public:
	virtual ~IEventHandler() { }

	virtual void handleEvent(SDL_Event* e) = 0;

protected:

private:

};


}

#endif
