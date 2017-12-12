
#ifndef Interfaces_Common_AProcessHandler_h
#define Interfaces_Common_AProcessHandler_h


// Library includes

// Project includes
#include "AProcess.h"

// Forward declarations

// Namespace declarations


namespace Common {


class AProcessHandler
{
public:
	virtual ~AProcessHandler() { }

public:
	void processAdd(AProcess *p) {
		mProcesses.push_front(p);
	}
	void processKill(AProcess *p) {
		if ( !p ) {
			//error("invalid <AProcess> provided - aborting...");
			return;
		}

		for ( ProcessList::iterator it = mProcesses.begin(); it != mProcesses.end(); ++it ) {
			if ( (*it) == p ) {
				(*it)->stop();

				mProcesses.remove(*it);
				break;
			}
		}
	}
	void processStart(AProcess *p) {
		if ( !p ) {
			//error("invalid <AProcess> provided - aborting...");
			return;
		}

		if ( !contains(p) ) {
			processAdd(p);
		}

		p->restart();
	}

	void runProcesses() {
		// run processes
		for ( ProcessList::iterator it = mProcesses.begin(); it != mProcesses.end(); ++it ) {
			(*it)->run();
		}

		// remove stopped processes
 		for ( ProcessList::iterator it = mProcesses.begin(); it != mProcesses.end(); ++it ) {
			if ( (*it)->keepAlive() ) {
				continue;
			}

			(*it)->stop();

			mProcesses.remove(*it);
		}
	}

protected:
	typedef std::list<Common::AProcess*> ProcessList;

protected:
	bool contains(AProcess *p) {
		for ( ProcessList::iterator it = mProcesses.begin(); it != mProcesses.end(); ++it ) {
			if ( (*it) == p ) {
				return true;
			}
		}

		return false;
	}

protected:
	ProcessList	mProcesses;

private:

};


}


#endif