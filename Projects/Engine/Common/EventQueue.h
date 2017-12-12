
#ifndef _Common_EventQueue_h_
#define _Common_EventQueue_h_


// Library includes
#include <queue>

// Project includes
#include <Interfaces/Common/Event.h>

// Forward declarations

// Namespace declarations


namespace Common {


class EventQueue
{
public:
	EventQueue();
	~EventQueue();

public:
	const Event& dequeue();
	bool empty() const;
	void enqueue(const Event& e);

protected:

private:
	typedef std::queue<Event> Events;

private:
	Events	mEvents;
};


}


#endif
