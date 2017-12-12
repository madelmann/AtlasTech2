
#ifndef _Interfaces_Core_ICoreManagement_h_
#define _Interfaces_Core_ICoreManagement_h_


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace Core {


class ICoreManagement
{
public:
	virtual ~ICoreManagement() { }

public:
	virtual std::string getSceneName() const = 0;
	virtual bool loadScene(const std::string& scene) = 0;
	virtual bool unloadScene() = 0;
};


}


#endif
