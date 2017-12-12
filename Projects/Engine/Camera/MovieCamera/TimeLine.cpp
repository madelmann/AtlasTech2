
// Header
#include "TimeLine.h"

// Library includes

// Project includes

// Namespace declarations


namespace CameraSpace {


TimeLine::TimeLine()
{
}

TimeLine::~TimeLine()
{
	for ( ActionMap::iterator it = mActions.begin(); it != mActions.end(); ++it ) {
		for ( ActionList::iterator lIt = (*it).second.begin(); lIt != (*it).second.end(); ++lIt ) {
			delete (*lIt);
		}

		(*it).second.clear();
	}

	mActions.clear();
}

void TimeLine::addAction(float time, CameraSpace::Action *action)
{
	mActions[time].push_back(action);
}

void TimeLine::cleanup()
{
	//for ( ActionMap::terator it = mActions.begin(); it != mActions.end(); ++it ) {
	//	if ( (*it).first < time ) {
	//		actions = (*it).second;
	//	}
	//}
}

TimeLine::ActionList TimeLine::getActions(float time) const
{
	ActionList actions;

	for ( ActionMap::const_iterator it = mActions.begin(); it != mActions.end(); ++it ) {
		if ( (*it).first < time ) {
			actions = (*it).second;
		}
	}

	return actions;
}


}
