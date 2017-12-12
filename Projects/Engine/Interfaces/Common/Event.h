
#ifndef _Interfaces_Common_Event_h_
#define _Interfaces_Common_Event_h_


// Library includes
#include <windows.h>

// Project includes
//#include "Types.h"

// Forward declarations

// Namespace declarations


namespace Common {


class Event
{
public:
	Event(UINT msg, WPARAM w, LPARAM l)
	: Handled(false),
	  uMsg(msg),
	  wParam(w),
	  lParam(l)
	{ }

	bool	Handled;
	UINT	uMsg;
	WPARAM	wParam;
	LPARAM	lParam;

protected:

private:

};


}

#endif
