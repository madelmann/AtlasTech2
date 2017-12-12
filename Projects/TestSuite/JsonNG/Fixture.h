
#ifndef Testing_JsonNG_Fixture_h
#define Testing_JsonNG_Fixture_h


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Framework/TestFixture.h>

// Forward declarations

// Namespace declarations


namespace Testing {
namespace JsonNG {


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
