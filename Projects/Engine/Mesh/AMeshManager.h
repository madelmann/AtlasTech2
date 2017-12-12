
#ifndef Mesh_AMeshManager_h
#define Mesh_AMeshManager_h


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Mesh/IMeshManager.h>

// Forward declarations

// Namespace declarations


namespace Mesh {


class AMeshManager : public IMeshManager,
					 protected Common::Logger
{
public:
	AMeshManager(const Common::Logger *p, const std::string& name);
	virtual ~AMeshManager() { }

public:
	// Common::AManager implementation
	//{
	virtual void init();
	virtual void manage();
	virtual void shutdown();
	virtual void update(float elapsedTime);
	//}

	// IMeshManager implementation
	//{
	bool handleFormat(const std::string& format) const;
	//}

protected:
	typedef std::list<Core::RenderObject*> RenderObjectList;

protected:
	std::string mFormat;

private:

};


}


#endif
