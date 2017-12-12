
#ifndef Interfaces_Sound_IManager_h
#define Interfaces_Sound_IManager_h


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/AManager.h>

// Forward declarations

// Namespace declarations


namespace SoundSpace {

// Forward declarations
class Sound;

class IManager : public Common::AManager
{
public:
	virtual ~IManager() { }

public:
	virtual Sound* create(const std::string& filename) = 0;
	virtual Sound* create2D(const std::string& filename) = 0;
	virtual Sound* create3D(const std::string& filename) = 0;
};


}


#endif
