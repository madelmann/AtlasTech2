
#ifndef Camera_MovieCamera_MovieCamera_h
#define Camera_MovieCamera_MovieCamera_h


// Library includes
#include <map>

// Project includes
#include <Camera/Viewer.h>
#include <Interfaces/Common/AManager.h>
#include <Interfaces/Camera/IViewer.h>
#include "TimeLine.h"

// Forward declarations

// Namespace declarations


namespace CameraSpace {


class MovieCamera : public Viewer,
					public Common::AManager
{
public:
	MovieCamera();
	virtual ~MovieCamera();

	// Common::AManager implementation
	// {
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);
	// }

	void addAction(float time, Action *action);
	//void look();

protected:

private:
	typedef std::map<int, TimeLine> TimeLineMap;

private:
	void loadConfig();
	void process(Action *action);

private:
	float		mLastFrameTime;
	float		mTime;
	TimeLineMap	mTimeLines;
};


}


#endif
