
#ifndef _Camera_MovieCamera_TimeLine_h_
#define _Camera_MovieCamera_TimeLine_h_


// Library includes
#include <list>
#include <map>

// Project includes
#include "Action.h"

// Forward declarations

// Namespace declarations


namespace CameraSpace {


class TimeLine
{
public:
	typedef std::list<Action*> ActionList;

public:
	TimeLine();
	~TimeLine();

	void addAction(float time, Action *action);
	void cleanup();
	ActionList getActions(float time) const;

protected:

private:
	typedef std::map<float, ActionList> ActionMap;

private:
	ActionMap	mActions;
};


}


#endif
