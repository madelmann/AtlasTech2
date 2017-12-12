
#ifndef _Vehicle_VehicleManager_h_
#define _Vehicle_VehicleManager_h_


// Library includes
#include <list>
#include <string>

// Project includes
#include <Common\Logger.h>
#include <Interfaces\Common\AManager.h>

// Forward declarations
class IMediaPathManager;
namespace Mesh {
	class IObjectManager;
}
namespace Scene {
	class IManager;
}
namespace Terrain {
	class IManager;
}
class Vehicle;

// Namespace declarations


class VehicleManager : public Common::AManager,
					   private Common::Logger
{
public:
	VehicleManager(const Common::Logger *p);
	~VehicleManager();

public:
	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime = 0.f);
	//}

	// Connectors
	//{
	void connectMediaPathManager(IMediaPathManager *m);
	void connectObjectManager(Mesh::IObjectManager *m);
	void connectSceneManager(Scene::IManager *m);
	void connectTerrainManager(Terrain::IManager *m);
	//}

	// Providers
	// {
	// }

	Vehicle* create(const std::string &filename);
	Vehicle* find(const std::string &filename);

protected:

private:
	IMediaPathManager		*mMediaPathManager;
	Mesh::IObjectManager	*mObjectManager;
	Scene::IManager			*mSceneManager;
	Terrain::IManager		*mTerrainManager;

	std::list<Vehicle*>	mVehicles;
};


#endif
