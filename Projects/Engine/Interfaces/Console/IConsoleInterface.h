
#ifndef _Interfaces_Console_IConsoleInterface_h_
#define _Interfaces_Console_IConsoleInterface_h_


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Console {

// Forward declarations
class IConsolePlugin;

class IConsoleInterface
{
public:
	virtual ~IConsoleInterface() { }

public:
	virtual void registerPlugin(IConsolePlugin *) = 0;
};


}


#endif
