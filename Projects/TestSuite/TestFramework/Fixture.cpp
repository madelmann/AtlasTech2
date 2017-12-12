
// Header
#include "Fixture.h"

// Library includes

// Project includes
#include "TestFrameworkTest.h"

// Namespace declarations


namespace Testing {


Fixture::Fixture(const Common::Logger *p)
: TestFixture("Testing"),
  Common::Logger(p, "TestFramework")
{
}

void Fixture::setup()
{
	TestFrameworkTest* frameworkTest = new TestFrameworkTest(this);
	add(frameworkTest);
}

void Fixture::teardown()
{
}


}
