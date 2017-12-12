
#ifndef Interfaces_Manager_AManager_h
#define Interfaces_Manager_AManager_h


// Library includes

// Project includes
#include "AComponent.h"
#include "ADependency.h"
#include "Version.h"

// Forward declarations

// Namespace declarations


namespace Common {


class AManager : public AComponent,
				 public ADependency,
				 public Version
{
public:
	virtual ~AManager() { }

public:
	virtual void clear() = 0;
	virtual void init() = 0;
	virtual void manage() = 0;
	virtual void update(float elapsedTime) = 0;

public:
	virtual void reset() {
		clear();
		init();
	}
};


}


#endif
