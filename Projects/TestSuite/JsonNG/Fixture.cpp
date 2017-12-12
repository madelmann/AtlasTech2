
// Header
#include "Fixture.h"

// Library includes

// Project includes
#include "Framework.h"

// Namespace declarations


namespace Testing {
namespace JsonNG {


Fixture::Fixture(const Common::Logger *p)
: TestFixture("JsonNG::Fixture"),
  Common::Logger(p, "JsonNG::Framework")
{
}

void Fixture::setup()
{
	add(new Framework(this));
}

void Fixture::teardown()
{
}


}
}
