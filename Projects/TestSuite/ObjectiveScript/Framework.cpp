
// Header
#include "Framework.h"

// Library includes

// Project includes
#include <Media/PathManager.h>
#include <ObjectiveScript/Script.h>

// Namespace declartations


namespace Testing {
namespace ObjectiveScript {


Framework::Framework(const Common::Logger *p)
: Common::Logger(p, "Framework"),
  mMediaPathManager(0),
  mPrinter(0),
  mVirtualMachine(0)
{
}

void Framework::process()
{
	TEST(test1);
	teardown();

	setup();
	TEST(test2);
}

void Framework::setup()
{
	mMediaPathManager = new MediaPathManager(this, "base");

	mPrinter = new Printer();
	mPrinter->connectPrinter(this);

	mVirtualMachine = new ::ObjectiveScript::VirtualMachine();
	mVirtualMachine->connectPrinter(mPrinter);
	mVirtualMachine->setBaseFolder(mMediaPathManager->getBasePath() + mMediaPathManager->getScripts());
}

void Framework::teardown()
{
	delete mMediaPathManager;
	delete mPrinter;
	delete mVirtualMachine;
}

void Framework::test1()
{
	::ObjectiveScript::Script *script = mVirtualMachine->create(
		mMediaPathManager->buildScriptPath("inheritance.os")
	);
	{
		script->execute("test1()");
	}
	delete script;
}

void Framework::test2()
{
	::ObjectiveScript::Script *script = mVirtualMachine->create(
		mMediaPathManager->buildScriptPath("startup.os")
	);
	{
		//script->execute("getName");
		//script->execute("test_assert");
		//script->execute("test_comments");
		script->execute("test_executeMethod");
		//script->execute("test_localVar");
		script->execute("test_staticLocalVar");
	}
	delete script;
}

}
}
