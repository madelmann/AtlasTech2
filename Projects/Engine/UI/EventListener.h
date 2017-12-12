
#ifndef _UI_EventListerner_h_
#define _UI_EventListerner_h_


// Library includes

// Project includes
#include "Event.h"

// Forward declarations

// Namespace declarations


namespace UI {

class EventListener
{
public:
    virtual void actionPerformed(const Event &event) = 0;

protected:

private:

};

}


#endif
