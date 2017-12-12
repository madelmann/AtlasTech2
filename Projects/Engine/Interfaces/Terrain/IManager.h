
#ifndef Interfaces_Terrain_IManager_h
#define Interfaces_Terrain_IManager_h


// Library inludes

// Project includes
#include <Interfaces/Common/AManager.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Terrain {


class IManager : public Common::AManager
{
public:
	virtual ~IManager() { }

public:
	virtual vector3f heightAt(const vector3f& pos) = 0;
	virtual vector3f normalAt(const vector3f& pos) = 0;

	virtual void render() = 0;
};


}

#endif
