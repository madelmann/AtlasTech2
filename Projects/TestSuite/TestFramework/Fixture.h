
#ifndef _Testing_TestFramework_Fixture_h_
#define _Testing_TestFramework_Fixture_h_


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Framework/TestFixture.h>

// Forward declarations

// Namespace declarations
using namespace Testing;


namespace Testing {


class Fixture : public TestFixture,
				private Common::Logger
{
public:
	Fixture(const Common::Logger *p);

public:
	void setup();
	void teardown();

protected:

private:

};


}


#endif _Testing_TestFramework_Fixture_h_
