
#ifndef _UI_Event_h_
#define _UI_Event_h_


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace UI {

// Forward declarations
class Frame;


class Event
{
public:
	Event(Frame *frame);
	Event(const Event& copy);
	Event &operator= (const Event& copy);

	const std::string& getCallback() const;
	Frame* getSource() const;

protected:

private:
	std::string	mCallback;
	Frame		*mSource;
};

}


#endif
