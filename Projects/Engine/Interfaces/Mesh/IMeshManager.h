
#ifndef _Interfaces_Mesh_IMeshManager_h_
#define _Interfaces_Mesh_IMeshManager_h_


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/AManager.h>

// Forward declarations
namespace Core {
	class RenderObject;
}

// Namespace declarations


namespace Mesh {


class IMeshManager : public Common::AManager
{
public:
	virtual ~IMeshManager() { }

public:
	virtual Core::RenderObject* create(const std::string& name) = 0;
	virtual Core::RenderObject* find(const std::string& name) = 0;
	virtual bool handleFormat(const std::string& format) const = 0;
};


}


#endif
