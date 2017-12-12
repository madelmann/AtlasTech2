
#ifndef Testing_JsonNG_FrameworkTest_h
#define Testing_JsonNG_FrameworkTest_h


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Framework/GenericTest.h>

// Forward declarations

// Namespace declarations


namespace Testing {
namespace JsonNG {


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
	void StorageTest();
	void TransactionTest();
	void ValueTest();
};


}
}


#endif
