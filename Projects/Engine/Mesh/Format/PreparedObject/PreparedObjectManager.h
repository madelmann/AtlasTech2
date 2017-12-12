
#ifndef Mesh_Format_PreparedObject_PreparedObjectManager_h
#define Mesh_Format_PreparedObject_PreparedObjectManager_h


// Library includes
#include <list>

// Project includes
#include <Mesh/AMeshManager.h>

// Forward declarations

// Namespace declarations


namespace Mesh {

// Forward declarations
class IObjectManager;
class PreparedObject;

class PreparedObjectManager : public Mesh::AMeshManager
{
public:
	PreparedObjectManager(const Common::Logger *p);
	~PreparedObjectManager();

public:
	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	void update(float elapsedTime);
	//}

	// IMeshManager implementation
	//{
	Core::RenderObject* create(const std::string& filename);
	Core::RenderObject* find(const std::string& filename);
	//}

	void connectMeshManager(IObjectManager *m);

protected:

private:
	typedef std::list<PreparedObject*> PreparedObjectList;

private:
	PreparedObjectList	mPreparedObjects;

	IObjectManager	*mMeshManager;
};


}


#endif
