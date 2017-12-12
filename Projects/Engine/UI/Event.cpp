
// Header
#include "Event.h"

// Library includes

// Project includes
#include "Frame.h"

// Namespace declarations


namespace UI {

Event::Event(Frame *frame)
: mCallback(""),
  mSource(frame)
{
}

Event::Event(const Event& copy)
{
	this->operator = (copy);
}

Event& Event::operator= (const Event& copy)
{
	if ( this != &copy ) {
		mCallback = copy.mCallback;
		mSource = copy.mSource;
	}

	return *this;
}

const std::string& Event::getCallback() const
{
	return mCallback;
}

Frame* Event::getSource() const
{
	return mSource;
}

}
