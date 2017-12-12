
#ifndef Testing_ObjectiveScript_Fixture_h
#define Testing_ObjectiveScript_Fixture_h


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Framework/TestFixture.h>

// Forward declarations

// Namespace declarations


namespace Testing {
namespace ObjectiveScript {


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
}


#endif
