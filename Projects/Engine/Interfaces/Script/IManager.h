
#ifndef Interfaces_Script_IManager_h
#define Interfaces_Script_IManager_h


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/AManager.h>

// Forward declarations
namespace ObjectiveScript {
	class Script;
}

// Namespace declarations


namespace ScriptSpace {

class IManager : public Common::AManager
{
public:
	virtual ~IManager() { }

public:
	virtual ObjectiveScript::Script* create(const std::string& filename) = 0;
};


}


#endif
