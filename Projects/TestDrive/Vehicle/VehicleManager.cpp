
// Header
#include "VehicleManager.h"

// Project includes
#include "Vehicle.h"
#include <Core\Configuration\Configurator.h>
#include <Interfaces\Media\IMediaPathManager.h>
#include <Interfaces\Mesh\IObjectManager.h>
#include <Interfaces\Scene\IManager.h>
#include <Interfaces\Terrain\IManager.h>
#include <Math\Normals.h>
#include <Parsers\LineParser.h>
#include <Renderer\RenderObject.h>
#include <Tools/Files.h>

// Namespace declarations


VehicleManager::VehicleManager(const Common::Logger *p)
: Common::Logger(p, "VehicleManager"),
  mMediaPathManager(0),
  mObjectManager(0),
  mSceneManager(0),
  mTerrainManager(0)
{
	debug("Starting...");
}

VehicleManager::~VehicleManager()
{
	shutdown();
}

void VehicleManager::clear()
{
	debug("Clearing...");

	for ( std::list<Vehicle*>::iterator it = mVehicles.begin(); it != mVehicles.end(); it++ ) {
		delete (*it);
	}

	mVehicles.clear();

	mIsReady = false;
}

void VehicleManager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mMediaPathManager);
	assert(mObjectManager);
	assert(mSceneManager);
	assert(mTerrainManager);

	mConfigurationComplete = true;
}

void VehicleManager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void VehicleManager::connectObjectManager(Mesh::IObjectManager *m)
{
	mObjectManager = m;
}

void VehicleManager::connectSceneManager(Scene::IManager *m)
{
	mSceneManager = m;
}

void VehicleManager::connectTerrainManager(Terrain::IManager *m)
{
	mTerrainManager = m;
}

Vehicle* VehicleManager::create(const std::string &filename)
{
	debug("load('" + filename + "')");

	if ( filename.empty() ) {
		return 0;
	}

	Vehicle *vehicle = find(filename);
	if ( vehicle ) {
		return vehicle;
	}

	std::string file = mMediaPathManager->getGame() + "\\"
					   + mMediaPathManager->getMaps() + "\\"
					   + mMediaPathManager->getMap() + "\\"
					   + "Vehicles" + "\\"
					   + filename;

	if ( !Tools::Files::exists(file) ) {
		file = mMediaPathManager->getGame() + "\\"
			 + "Vehicles" + "\\"
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			warn("file '" + filename + "' not found!");
			return 0;
		}
	}

	Core::Configuration::Configurator config(this, file);
	if ( !config.success() ) {
		error("error while parsing configuration file '" + file + "'!");
		return 0;
	}

	vehicle = new Vehicle();	
	vehicle->connectTerrainManager(mTerrainManager);

	vehicle->setAirDecceleration(config.getValue("air_decceleration").toFloat());
	vehicle->setAxle(config.getValue("axle").toVector3f());
	vehicle->setBreakDecceleration(config.getValue("break_decceleration").toFloat());
	vehicle->cameraOffset(config.getValue("camera_offset").toVector3f());
	vehicle->setCurveAngle(config.getValue("curve_angle").toFloat());
	vehicle->setMass(config.getValue("mass").toFloat());
	vehicle->setMaxAcceleration(config.getValue("max_acceleration").toFloat());
	vehicle->setMaxCurveAngle(config.getValue("max_curve_angle").toFloat());
	vehicle->setMaxSpeed(config.getValue("max_speed").toFloat());
	vehicle->setName(config.getValue("name").toString());

	std::string meshStr = config.getValue("mesh").toString();

	Core::RenderObject *mesh = mObjectManager->create(meshStr);
	if ( !mesh ) {
		error("could not load '" + meshStr + "'");
		return 0;
	}

	Scene::SceneGraphObject *sgo = mSceneManager->create(mesh);
	vehicle->sceneGraphObject(sgo);
	mSceneManager->attach(sgo, Core::invalidID, true);

	mVehicles.push_back(vehicle);

	return vehicle;
}

Vehicle* VehicleManager::find(const std::string &filename)
{
	if ( filename.empty() ) {
		return 0;
	}

	for ( std::list<Vehicle*>::iterator it = mVehicles.begin(); it != mVehicles.end(); it++ ) {
		if ( Tools::StringCompareI(filename, (*it)->getName()) ) {
			return (*it);
		}
	}

	return 0;
}

void VehicleManager::init()
{
	debug("Initializing...");

	mIsReady = true;
}

void VehicleManager::manage()
{
	// nothing to do here
}

void VehicleManager::shutdown()
{
	debug("Stopping...");

	clear();
}

void VehicleManager::update(float elapsedTime)
{
	for ( std::list<Vehicle*>::iterator it = mVehicles.begin(); it != mVehicles.end(); it++ ) {
		(*it)->update(elapsedTime);
	}
}
