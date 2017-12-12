
#ifndef _Interfaces_Material_IManager_h_
#define _Interfaces_Material_IManager_h_


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/AManager.h>

// Forward declarations

// Namespace declarations


namespace MaterialSpace {

// Forward declarations
class Material;

class IManager : public Common::AManager
{
public:
	virtual ~IManager() { }

public:
	virtual Material* create(const std::string& filename, const std::string& path = "") = 0;
	virtual Material* find(const std::string& material) = 0;
};


}


#endif
