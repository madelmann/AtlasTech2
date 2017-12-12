
// Header
#include "IdGenerator.h"

// Library includes

// Project includes

// Namespace declarations


namespace Core {


IdGenerator::IdGenerator()
{
	// just to make sure that we have the same behaviour
	// like after a reset
	reset();
}

AEid IdGenerator::generate()
{
	return mLastId++;
}

void IdGenerator::reset()
{
	mLastId = 0;
}

}
