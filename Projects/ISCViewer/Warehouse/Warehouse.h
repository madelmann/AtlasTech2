
#ifndef Warehouse_Warehouse_h
#define Warehouse_Warehouse_h


// Library includes
#include <list>
#include <map>

// Project includes
#include "Types.h"
#include <Common\Logger.h>
#include <Interfaces\Common\AManager.h>
#include <Math\Vector3.h>

// Forward declarations
namespace Mesh {
	class IObjectManager;
}
namespace Scene {
	class IManager;
}

// Namespace declarations


namespace WarehouseSpace {

// Forward declarations
class Storage;

class Warehouse : public Common::AManager,
				  private Common::Logger
{
public:
	Warehouse(const Common::Logger *p);
	~Warehouse();

public:
	// Common::AManager implementation
	// {
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);
	// }

	void connectObjectManager(Mesh::IObjectManager *m);
	void connectSceneManager(Scene::IManager *m);

protected:

private:
	class VertexDistance
	{
	public:
		VertexDistance()
		{ }
		VertexDistance(const Vertex *source, const Vertex *dest, vector3f dist)
		: mDestination(dest),
		  mDistance(dist),
		  mSource(source)
		{ }

		const Vertex* getDestination() const { return mDestination; }
		const vector3f& getDistance() const { return mDistance; }
		const Vertex* getSource() const { return mSource; }

	private:
		const Vertex	*mDestination;
		vector3f		mDistance;
		const Vertex	*mSource;
	};

	typedef std::list<Edge> EdgeList;
	typedef std::list<Vertex> VertexList;
	typedef std::map<Vertex*, vector3f> VertexMap;
	typedef std::map<const Vertex*, VertexDistance> VertexDistanceMap;

private:
	const Edge* findIncomingEdge(int levelId, int posId, int alignment) const;
	const Edge* findOutgoingEdge(int levelId, int posId, int alignment) const;
	const Vertex* findVertexByPosition(int levelId, int posId) const;
	void position();
	void process();

private:
	Mesh::IObjectManager	*mObjectManager;
	Scene::IManager			*mSceneManager;
	Storage					*mStorage;

	EdgeList	mEdges;
	VertexList	mVertices;
	VertexMap	mVertexMap;
	VertexDistanceMap	mVertexDistances;
};


}


#endif
