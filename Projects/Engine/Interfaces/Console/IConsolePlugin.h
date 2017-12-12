
#ifndef _Interfaces_Console_IConsolePlugin_h_
#define _Interfaces_Console_IConsolePlugin_h_


// Library includes
#include <string>

// Project includes
#include <Interfaces/Core/Name.h>
#include <Parsers/Valueizer.h>

// Forward declarations

// Namespace declarations


namespace Console {

// Forward declarations
class IPrinter;
class Settings;

class IConsolePlugin : public Core::Name
{
public:
	virtual ~IConsolePlugin() { }

public:
	virtual void execute(Valueizer *v) = 0;

public:
	// Connectors
	// {
	void connectPrinter(IPrinter *p) { mPrinter = p; }
	void connectSettings(Settings *s) { mSettings = s; }
	// }

protected:
	IConsolePlugin(const std::string& name)
	: Core::Name(name)
	{ }

protected:
	IPrinter	*mPrinter;
	Settings	*mSettings;
};


}


#endif
