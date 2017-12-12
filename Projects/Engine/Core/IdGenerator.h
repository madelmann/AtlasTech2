
#ifndef Core_IdGenerator_h
#define Core_IdGenerator_h


// Library includes

// Project includes
#include <Interfaces/Core/Types.h>

// Forward declarations

// Namespace declarations


namespace Core {


class IdGenerator
{
public:
	IdGenerator();

public:
	AEid generate();

protected:

private:
	void reset();

private:
	AEid mLastId;
};


}


#endif
