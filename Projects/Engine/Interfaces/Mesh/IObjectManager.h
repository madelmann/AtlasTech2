
#ifndef _Interfaces_Mesh_IObjectManager_h_
#define _Interfaces_Mesh_IObjectManager_h_


// Library includes

// Project includes
#include "IMeshManager.h"

// Forward declarations

// Namespace declarations


namespace Mesh {


class IObjectManager : public IMeshManager
{
public:
	virtual ~IObjectManager() { }

public:
	virtual void addPlugin(IMeshManager *) = 0;
};


}


#endif
