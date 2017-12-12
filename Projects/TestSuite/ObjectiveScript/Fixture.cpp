
// Header
#include "Fixture.h"

// Library includes

// Project includes
#include "Framework.h"

// Namespace declarations


namespace Testing {
namespace ObjectiveScript {


Fixture::Fixture(const Common::Logger *p)
: TestFixture("ObjectiveScript::Fixture"),
  Common::Logger(p, "ObjectiveScript::Framework")
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
