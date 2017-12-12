
// Header
#include "EventQueue.h"

// Library includes

// Project includes

// Namespace declarations


namespace Common {


EventQueue::EventQueue()
{
}

EventQueue::~EventQueue()
{
}

const Event& EventQueue::dequeue()
{
	const Event& e = mEvents.front();

	mEvents.pop();

	return e;
}

bool EventQueue::empty() const
{
	return mEvents.empty();
}

void EventQueue::enqueue(const Event& e)
{
	mEvents.push(e);
}


}
