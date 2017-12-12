
#ifndef _Interfaces_Console_ICommandExecutor_h_
#define _Interfaces_Console_ICommandExecutor_h_


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace Console {

// Forward declarations

class ICommandExecutor
{
public:
	virtual ~ICommandExecutor() { }

public:
	virtual void execute(const std::string& command) = 0;
};


}


#endif
