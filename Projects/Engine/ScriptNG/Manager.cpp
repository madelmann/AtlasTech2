
// Header
#include "Manager.h"

// Library includes

// Project includes
#include <Core/IdGenerator.h>
#include <Interfaces/Console/IConsoleInterface.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <ObjectiveScript/Analyser.h>
#include <ObjectiveScript/Exceptions.h>
#include <ObjectiveScript/Object.h>
#include <ObjectiveScript/Preprocessor.h>
#include <ObjectiveScript/Script.h>
#include <ObjectiveScript/VirtualMachine.h>
#include <ScriptNG/Plugins/ConsolePlugin.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace ScriptSpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "ScriptManager"),
  mConsole(0),
  mIdGenerator(0),
  mMediaPathManager(0),
  mVirtualMachine(0)
{
	debug("Starting...");
}

Manager::~Manager()
{
	shutdown();
}

void Manager::clear()
{
	debug("Clearing...");

	for ( ScriptMap::iterator it = mScripts.begin(); it != mScripts.end(); ++it ) {
		aeDelete( it->second );
	}
	mScripts.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	mVirtualMachine = new VirtualMachine();
	mVirtualMachine->connectPrinter(&mPrinter);
	mVirtualMachine->setBaseFolder(mMediaPathManager->getBasePath() + mMediaPathManager->getScripts());

	assert(mConsole);
	assert(mIdGenerator);
	assert(mMediaPathManager);
	assert(mVirtualMachine);

	{	// Console plugin
		ConsolePlugin *plugin = new ConsolePlugin();
		plugin->connectScriptManager(this);

		mConsole->registerPlugin(plugin);
	}

	mConfigurationComplete = true;
}

void Manager::connectConsole(IConsoleInterface *c)
{
	mConsole = c;
}

void Manager::connectConsolePrinter(Console::IPrinter *p)
{
	mPrinter.connectLogger(this);
	mPrinter.connectPrinter(p);
}

void Manager::connectIdGenerator(Core::IdGenerator *g)
{
	mIdGenerator = g;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

ObjectiveScript::Script* Manager::create(const std::string& filename)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	std::string barename = Tools::Files::RemoveFileExt(filename);
	std::string fullname = mMediaPathManager->buildScriptPath(barename + ".os");

	if ( !Tools::Files::exists(fullname) ) {
		error("file '" + barename + "' does not exist!");
		return 0;
	}

	Script* fromRepo = find(fullname);
	if ( fromRepo ) {
		debug("getting '" + fullname + "' from repository");
		return fromRepo;
	}

	Script *script = mVirtualMachine->create(fullname);
	if ( !script ) {
		error("error while loading script '" + fullname + "'!");
		return 0;
	}

	mScripts.insert(
		std::make_pair<std::string, Script*>(fullname, script)
	);

	return script;
}

Script* Manager::find(const std::string name)
{
	ScriptMap::iterator it = mScripts.find(name);

	if ( it != mScripts.end() ) {
		return it->second;
	}

	return 0;
}

void Manager::init()
{
	debug("Initializing...");

	mIsReady = true;
}

void Manager::manage()
{
	// nothing to do for now
}

Settings* Manager::provideSettings()
{
	return &mSettings;
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();

	delete mVirtualMachine;
}

void Manager::update(float elapsedTime)
{
	ObjectiveScript::VariablesList params;

	static ObjectiveScript::Variable time("time", "number");
	time.value(Tools::toString(elapsedTime));
	params.push_back(time);

	for ( ScriptMap::iterator it = mScripts.begin(); it != mScripts.end(); ++it ) {
		if ( it->second->hasMethod("update", params) ) {
			try {
				it->second->execute("update", params);
			}
			catch ( ObjectiveScript::Exception &e ) {
				error(e.what());
			}
		}
	}
}


}
