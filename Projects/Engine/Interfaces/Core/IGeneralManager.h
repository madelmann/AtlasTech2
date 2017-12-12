
#ifndef _Interfaces_Manager_IGeneralManager_h_
#define _Interfaces_Manager_IGeneralManager_h_


// Library includes

// Project includes
#include <Interfaces/Common/AManager.h>

// Forward declarations

// Namespace declarations


namespace Core {


class IGeneralManager: public Common::AManager
{
public:
	virtual ~IGeneralManager() { }

	virtual void add(Common::AManager *m) = 0;
};


}


#endif
