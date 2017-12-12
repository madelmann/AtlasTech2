
#ifndef ScriptNG_Manager_h
#define ScriptNG_Manager_h


// Library includes
#include <map>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Interfaces/Script/IManager.h>
#include <ObjectiveScript/Interfaces/IPrinter.h>
#include "Settings.h"

// Forward declarations
namespace Console {
	class IConsoleInterface;
	class IPrinter;
}
namespace Core {
	class IdGenerator;
}
class IMediaPathManager;
namespace ObjectiveScript {
	class Script;
	class VirtualMachine;
}

// Namespace declarations
using Console::IConsoleInterface;
using ObjectiveScript::Script;
using ObjectiveScript::VirtualMachine;


namespace ScriptSpace {

class Printer : public ObjectiveScript::IPrinter
{
public:
	Printer()
	: mPrinter(0),
	  mLogger(0)
	{ }

	~Printer()
	{
		delete mLogger;
	}

public:
	void connectLogger(const Common::Logger *p) {
		mLogger = new Common::Logger(p, "Printer");
	}
	void connectPrinter(Console::IPrinter *p) {
		mPrinter = p;
	}

	void log(const std::string& text) {
		mLogger->LogDebug(text, "ObjectiveScript", 0);
	}

	void print(const std::string& text) {
		mPrinter->print(text);
	}

private:
	Common::Logger		*mLogger;
	Console::IPrinter	*mPrinter;
};

class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

public:
	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);
	//}

	// IManager implementation
	// {
	ObjectiveScript::Script* create(const std::string& filename);
	// }

	// Connectors
	//{
	void connectConsole(IConsoleInterface *c);
	void connectConsolePrinter(Console::IPrinter *p);
	void connectIdGenerator(Core::IdGenerator *g);
	void connectMediaPathManager(IMediaPathManager *m);
	//}

	// Providers
	// {
	Settings* provideSettings();
	// }

protected:

private:
	typedef std::map<std::string, Script*> ScriptMap;

private:
	Script* find(const std::string name /*, add param list here to allow method overloading*/);

private:
	Printer		mPrinter;
	ScriptMap	mScripts;
	Settings	mSettings;
	VirtualMachine	*mVirtualMachine;

	IConsoleInterface	*mConsole;
	Core::IdGenerator	*mIdGenerator;
	IMediaPathManager	*mMediaPathManager;
};


}


#endif
