
#ifndef _Common_EventQueue_h_
#define _Common_EventQueue_h_


// Library includes
#include <queue>
#include <SDL2/SDL_events.h>

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {


class EventQueue
{
public:
	EventQueue();
	~EventQueue();

public:
	SDL_Event* dequeue();
	bool empty() const;
	void enqueue(SDL_Event* e);

protected:

private:
	typedef std::queue<SDL_Event*> Events;

private:
	Events	mEvents;
};


}


#endif
