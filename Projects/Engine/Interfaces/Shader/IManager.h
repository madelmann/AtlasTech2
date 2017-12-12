
#ifndef Interfaces_Shader_IManager_h
#define Interfaces_Shader_IManager_h


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/AManager.h>

// Forward declarations

// Namespace declarations


namespace ShaderSpace {

// Forward declarations
class Shader;

class IManager : public Common::AManager
{
public:
	virtual ~IManager() { }

public:
	virtual Shader* create(const std::string& filename, const std::string& path = "") = 0;
	virtual Shader* find(const std::string& shader) = 0;
};


}

#endif
