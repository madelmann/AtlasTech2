
#ifndef Arena_Network_ILocalPlayerManager_h
#define Arena_Network_ILocalPlayerManager_h


// Project includes
#include <Interfaces/Core/Types.h>
#include <Math/Vector3.h>


namespace Network {


class ILocalPlayerManager
{
public:
	virtual ~ILocalPlayerManager() { }

	virtual void initLocalPlayer(Core::AEid id, const vector3f& position) = 0;
};


}


#endif
