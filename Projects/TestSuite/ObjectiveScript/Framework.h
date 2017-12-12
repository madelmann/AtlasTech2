
#ifndef Testing_ObjectiveScript_FrameworkTest_h
#define Testing_ObjectiveScript_FrameworkTest_h


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Framework/GenericTest.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <ObjectiveScript/Interfaces/IPrinter.h>
#include <ObjectiveScript/VirtualMachine.h>

// Forward declarations

// Namespace declarations


namespace Testing {
namespace ObjectiveScript {


class Printer : public ::ObjectiveScript::IPrinter
{
public:
	Printer()
	: mLogger(0)
	{ }

	~Printer()
	{
		delete mLogger;
	}

public:
	void connectPrinter(const Common::Logger *p) {
		mLogger = new Common::Logger(p, "Printer");
	}

public:
	void print(const std::string& text) {
		mLogger->LogDebug(text, "ObjectiveScript", 0);
	}

private:
	Common::Logger *mLogger;
};


class Framework : private Common::Logger,
				  public GenericTest
{
public:
	Framework(const Common::Logger *p);

public:
	void process();
	void setup();
	void teardown();

private:
	// Tests
	void test1();
	void test2();

private:
	IMediaPathManager *mMediaPathManager;
	Printer *mPrinter;
	::ObjectiveScript::VirtualMachine *mVirtualMachine;
};


}
}


#endif
