
#ifndef Interfaces_Common_AProcess_h
#define Interfaces_Common_AProcess_h


// Library includes

// Project includes
#include "IEventHandler.h"

// Forward declarations

// Namespace declarations


namespace Common {


class AProcess : public IEventHandler
{
public:
	AProcess()
	: mIsRunning(false),
	  mKeepAlive(true)
	{ }
	virtual ~AProcess() {
		stop();
	}

public:
	virtual void run() = 0;

	virtual void stop() {
		mIsRunning = false;
		mKeepAlive = false;
	}

	bool isRunning() const {
		return mIsRunning;
	}
	bool keepAlive() const {
		return mKeepAlive;
	}

	void pause() {
		mIsRunning = false;
	}
	void restart() {
		mIsRunning = true;
		mKeepAlive = true;
	}

protected:

private:
	bool	mIsRunning;
	bool	mKeepAlive;
};


}


#endif
