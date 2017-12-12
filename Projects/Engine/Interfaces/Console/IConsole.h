
#ifndef _Interfaces_Console_IConsole_h_
#define _Interfaces_Console_IConsole_h_


// Library includes

// Project includes
#include "ICommandExecutor.h"
#include "IConsolePrinter.h"
#include "IConsoleInterface.h"

// Forward declarations

// Namespace declarations


namespace Console {


class IConsole : public ICommandExecutor,
				 public IConsoleInterface,
				 public IPrinter
{
public:
	virtual ~IConsole() { }

public:
	virtual void disable() = 0;
	virtual void enable() = 0;
	virtual void hide() = 0;
	virtual void show() = 0;
};


}


#endif
