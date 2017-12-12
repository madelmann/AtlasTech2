#ifndef _Interfaces_Console_IConsoleProgram_h_
#define _Interfaces_Console_IConsoleProgram_h_


// Library includes
#include <windows.h>

// Project includes
#include "IConsolePlugin.h"

// Forward declarations

// Namespace declaration


namespace Console {


class IConsoleProgram : public IConsolePlugin
{
public:
	virtual ~IConsoleProgram() { }

public:
	virtual void processKeyPress(WPARAM key) = 0;

protected:
	IConsoleProgram(const std::string& name)
	: IConsolePlugin(name)
	{ }

private:

};


}


#endif
