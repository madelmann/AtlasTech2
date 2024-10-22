
#ifdef _WIN32
// Memory leak check - Begin
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#include <iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
// Memory leak check - End
#endif


// Library includes
#include <list>

// Project includes
#include <Common/StdOutLogger.h>
#include <Tools/Strings.h>

// Fixtures
#include <JsonNG/Fixture.h>
#include <ObjectiveScript/Fixture.h>
#include <TestFramework/Fixture.h>

// Namespace declarations


typedef std::list<TestFixture*> FixtureList;


int main(int argc, const char* argv[])
{
#ifdef _WIN32
	// Memory leak detection
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// Memory leak detection
#endif

	bool executed = false;
	FixtureList mFixtures;
	Common::Logger *logger = 0;
	std::string toRun = "";
	bool show = false;

	if ( argc > 1 ) {
		for (int i = 1; i < argc; i++) {
			char buf[255];
			sprintf_s(buf, "%s", argv[i]);

			if ( Tools::StringCompare(buf, "-v") ) {
				logger = new Common::StdOutLogger();
			}
			else if ( Tools::StringCompare(buf, "--show") ) {
				show = true;
			}
			else {
				toRun = buf;
			}
		}
	}

	if ( !logger ) {
		logger = new Common::Logger();
	}

	try {
		Testing::Fixture testing(logger);
		mFixtures.push_back(&testing);

		//Testing::JsonNG::Fixture json(logger);
		//mFixtures.push_back(&json);

		Testing::ObjectiveScript::Fixture objectivescript(logger);
		mFixtures.push_back(&objectivescript);

		for ( FixtureList::iterator it = mFixtures.begin(); it != mFixtures.end(); ++it ) {
			if ( show ) {
				std::cout << (*it)->getName() << std::endl;
				continue;
			}

			if ( toRun.empty() || toRun == (*it)->getName() ) {
				executed = true;
				(*it)->run();
			}
		}
	}
	catch ( std::exception &e ) {
		std::cout << "Unhandled std::exception: " << e.what();
		logger->error("Unhandled std::exception: " + std::string(e.what()));
	}

	if ( !executed && !show ) {
		std::cout << "could not find fixture '" << toRun << "'!" << std::endl;
	}

	delete logger;
}