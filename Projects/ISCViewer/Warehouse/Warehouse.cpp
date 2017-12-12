
// Header
#include "Warehouse.h"

// Library includes

// Project includes
#include "Storage.h"
#include <Database\SQLResult.h>
#include <Interfaces\Mesh\IObjectManager.h>
#include <Interfaces\Scene\IManager.h>
#include <Parsers\Value.h>
#include <Renderer\RenderObject.h>
#include <Scene\SceneGraphObject.h>

// Namespace declarations
using Database::SQLEntry;
using Database::SQLRecord;
using Database::SQLResult;


namespace WarehouseSpace {


Warehouse::Warehouse(const Common::Logger *p)
: Common::Logger(p, "Warehouse"),
  mObjectManager(0),
  mSceneManager(0),
  mStorage(0)
{
	debug("Starting...");

	mStorage = new Storage(this);
}

Warehouse::~Warehouse()
{
	shutdown();
}

void Warehouse::clear()
{
	debug("Clearing...");
}

void Warehouse::configurationComplete()
{
	assert(!mConfigurationComplete);

	assert(mObjectManager);
	assert(mSceneManager);
	assert(mStorage);

	mConfigurationComplete = true;
}

void Warehouse::connectObjectManager(Mesh::IObjectManager *m)
{
	mObjectManager = m;
}

void Warehouse::connectSceneManager(Scene::IManager *m)
{
	mSceneManager = m;
}

const Edge* Warehouse::findIncomingEdge(int levelId, int posId, int alignment) const
{
	for ( EdgeList::const_iterator it = mEdges.begin(); it != mEdges.end(); ++it ) {
		if ( (*it).SourceLevelId == levelId && (*it).SourcePosId == posId && (*it).Alignment == alignment ) {
			return &(*it);
		}
	}

	return 0;
}

const Edge* Warehouse::findOutgoingEdge(int levelId, int posId, int alignment) const
{
	for ( EdgeList::const_iterator it = mEdges.begin(); it != mEdges.end(); ++it ) {
		if ( (*it).DestLevelId == levelId && (*it).DestPosId == posId && (*it).Alignment == alignment ) {
			return &(*it);
		}
	}

	return 0;
}

const Vertex* Warehouse::findVertexByPosition(int levelId, int posId) const
{
	for ( VertexList::const_iterator it = mVertices.begin(); it != mVertices.end(); ++it ) {
		if ( (*it).LevelId == levelId && (*it).PosId == posId ) {
			return &(*it);
		}
	}

	return 0;
}

void Warehouse::init()
{
	debug("Initializing...");

	if ( !mStorage->open("base/maps/flat/warehouse.db") ) {
		error("could not open database!");
		return;
	}

	// load edges
	{
		SQLResult result = mStorage->query("select * from graph_edges");
		if ( result.empty() ) {
			error("query result is empty!");
			return;
		}

		for ( unsigned int r = 0; r < result.size(); ++r ) {
			SQLRecord record = result[r];

			Edge e;
			e.Alignment = Value(record["alignment"].Value).toInt();
			e.Angle = Value(record["angle"].Value).toInt();
			e.DestLevelId = Value(record["t_level_id"].Value).toInt();
			e.DestPosId = Value(record["t_pos_id"].Value).toInt();
			e.Length = Value(record["length"].Value).toInt();
			e.SourceLevelId = Value(record["s_level_id"].Value).toInt();
			e.SourcePosId = Value(record["s_pos_id"].Value).toInt();

			mEdges.push_back(e);
		}
	}

	// load vertices
	{
		SQLResult result = mStorage->query("select * from graph_vertices");
		if ( result.empty() ) {
			error("query result is empty!");
			return;
		}

		for ( unsigned int r = 0; r < result.size(); ++r ) {
			SQLRecord record = result[r];

			Vertex v;
			v.LevelId = Value(record["level_id"].Value).toInt();
			v.PosId = Value(record["pos_id"].Value).toInt();
			v.Type = static_cast<VertexType::E>(Value(record["type"].Value).toInt());

			mVertices.push_back(v);
		}
	}

	process();

	mIsReady = true;
}

void Warehouse::manage()
{
	// nothing to do here
}

void Warehouse::position()
{
	//mVertexMap
}

void Warehouse::process()
{
	// load meshes
	for ( VertexList::iterator it = mVertices.begin(); it != mVertices.end(); ++it ) {
		std::string mesh;

		switch ( (*it).Type ) {
			case VertexType::Box:
				mesh = "box";
				break;
			case VertexType::Crossing:
				mesh = "crossing";
				break;
			case VertexType::Flank:
				mesh = "flank";
				break;
			case VertexType::Place:
				mesh = "place";
				break;
			case VertexType::Parking:
				mesh = "parking";
				break;
		}

		(*it).RenderObject = mObjectManager->create(mesh + ".ao");
	}

	for ( EdgeList::const_iterator it = mEdges.begin(); it != mEdges.end(); ++it ) {
		const Vertex *dest = findVertexByPosition((*it).DestLevelId, (*it).DestPosId);
		const Vertex *source = findVertexByPosition((*it).SourceLevelId, (*it).SourcePosId);

		vector3f dist;

		switch ( (*it).Angle ) {
			case 0:
				dist = vector3f((*it).Length / 10.f, 0.f, 0.f);
				break;
			case 90:
				dist = vector3f(0.f, 0.f, (*it).Length / 10.f);
				break;
			case 180:
				dist = vector3f(-(*it).Length / 10.f, 0.f, 0.f);
				break;
			case 270:
				dist = vector3f(0.f, 0.f, -(*it).Length / 10.f);
				break;
			default:
				assert(!"invalid angle");
				break;
		}

		mVertexDistances[source] = VertexDistance(source, dest, dist);
	}

	position();
}

void Warehouse::shutdown()
{
	debug("Stopping...");

	clear();

	delete mStorage;
}

void Warehouse::update(float /*elapsedTime*/)
{
	// nothing to do here
}


}
