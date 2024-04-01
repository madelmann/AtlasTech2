
// Header
#include "MovieCamera.h"

// Library includes

// Project includes

// Namespace declarations


namespace CameraSpace {


MovieCamera::MovieCamera()
: mTime(0.f)
{
	mTimeLines[0] = TimeLine();
}

MovieCamera::~MovieCamera()
{
	shutdown();
}

void MovieCamera::addAction(float time, Action *action)
{
	mTimeLines[0].addAction(time, action);
}

void MovieCamera::clear()
{
	mTimeLines.clear();
}

void MovieCamera::configurationComplete()
{
	assert(!mConfigurationComplete);

	mConfigurationComplete = true;
}

void MovieCamera::init()
{
	loadConfig();

	mIsReady = true;
}

void MovieCamera::loadConfig()
{

}

//void MovieCamera::look()
//{
//}

void MovieCamera::manage()
{
	for ( TimeLineMap::iterator timeIt = mTimeLines.begin(); timeIt != mTimeLines.end(); ++timeIt ) {
		TimeLine::ActionList actions = timeIt->second.getActions(mTime);

		for ( TimeLine::ActionList::iterator actionIt = actions.begin(); actionIt != actions.end(); ++actionIt ) {
			process(*actionIt);
		}

		timeIt->second.cleanup();
	}
}

void MovieCamera::process(Action *action)
{
	if ( !action ) {
		return;
	}

	// get remaining action duration
	float duration = action->duration();

	switch ( action->type() ) {
		case Action::Type::Default:
			break;
		case Action::Type::Move: {
				MoveAction *a = static_cast<MoveAction*>(action);
				if ( !a ) {
					return;
				}

				vector3f actual = position();
				vector3f desired = a->position();

				vector3f tmp = (desired - actual) * mLastFrameTime + actual;

				position(tmp);
			}
			break;
		case Action::Type::Rotate: {
				RotateAction *a = static_cast<RotateAction*>(action);
				if ( !a ) {
					return;
				}
				vector3f actual = view();
				vector3f desired = a->rotation();

				vector3f tmp = (desired - actual) * mLastFrameTime + actual;
				view(tmp);
			}
			break;
	}

	// update remaining action duration
	action->duration(duration - mLastFrameTime);
}

void MovieCamera::shutdown()
{
	clear();
}

void MovieCamera::update(float elapsedTime)
{
	mLastFrameTime = elapsedTime;
	mTime += elapsedTime;
}


}
