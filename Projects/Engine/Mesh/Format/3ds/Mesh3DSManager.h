
#ifndef _Mesh_Format_3ds_Mesh3DSManager_h_
#define _Mesh_Format_3ds_Mesh3DSManager_h_


// Library includes

// Project includes
#include <Mesh/AMeshManager.h>

// Forward declarations

// Namespace declarations


namespace Mesh {

// Forward declarations
class Mesh3DS;

class Mesh3DSManager : public AMeshManager
{
public:
	Mesh3DSManager(const Common::Logger *p);
	~Mesh3DSManager();

public:
	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	//}

	// IMeshManager implementation
	//{
	Core::RenderObject* create(const std::string& filename);
	Core::RenderObject* find(const std::string& filename);
	//}

protected:

private:
	typedef std::list<Mesh3DS*> Mesh3DSList;

private:
	Mesh3DSList	mMeshes;
};


}


#endif
