
// Header
#include "VirtualMachine.h"

// Library includes

// Project includes
#include <ObjectiveScript/Interfaces/IPrinter.h>
#include "Analyser.h"
#include "Object.h"
#include "Preprocessor.h"
#include "Script.h"
#include "Tools.h"

// Namespace declarations


namespace ObjectiveScript {


VirtualMachine::VirtualMachine()
: mCounter(0),
  mPrinter(0)
{
}

VirtualMachine::~VirtualMachine()
{
	mObjects.clear();
}

std::string VirtualMachine::buildLibraryPath(const std::string& library) const
{
	std::string result = library;
	unsigned int npos = std::string::npos;

	do {
		npos = result.find_first_of(".");
		if ( npos != std::string::npos ) {
			result[npos] = '/';
		}
	} while ( npos != std::string::npos );

	result = mBaseFolder + "/" + result + ".os";
	return result;
}

void VirtualMachine::connectPrinter(IPrinter *p)
{
	mPrinter = p;
}

Script* VirtualMachine::create(const std::string& filename)
{
	os_debug("create('" + filename + "')");
	mPrinter->print("create('" + filename + "')");

	if ( filename.empty() ) {
		os_warn("invalid filename provided!");
		return 0;
	}

	Script *script = new Script(++mCounter);
	script->connectPrinter(mPrinter);

	Analyser a;
	a.process(filename);

	const std::list<std::string>& libraries = a.getLibraryReferences();
	ObjectBluePrintList objects = a.getObjects();

	for ( std::list<std::string>::const_iterator it = libraries.begin(); it != libraries.end(); ++it ) {
		loadLibrary((*it));
	}

	for ( ObjectBluePrintList::iterator it = objects.begin(); it != objects.end(); ++it ) {
		mRepository.addBlueprint((*it));

		if ( it->filename() == filename && it->objectType() == "Main" ) {
			mObjects.insert(std::make_pair<std::string, Object>(
				it->objectType(),
				mRepository.createInstance(it->objectType(), "main")
			));

			ObjectMap::iterator object = mObjects.find(it->objectType());
			assert(object != mObjects.end());

			script->assign(&object->second);
		}
	}

	script->init(&mRepository);
	script->construct();

	return script;
}

void VirtualMachine::loadLibrary(const std::string& library)
{
	os_debug("Loading additional library file '" + library + "'");

	try {
		Analyser a;
		a.process(buildLibraryPath(library));

		const std::list<std::string>& libraries = a.getLibraryReferences();
		ObjectBluePrintList objects = a.getObjects();

		for ( std::list<std::string>::const_iterator it = libraries.begin(); it != libraries.end(); ++it ) {
			loadLibrary((*it));
		}

		for ( ObjectBluePrintList::iterator it = objects.begin(); it != objects.end(); ++it ) {
			mRepository.addBlueprint((*it));
		}
	}
	catch ( std::exception& e ) {
		os_error("caught exceptions: " << e.what());
	}
}

void VirtualMachine::setBaseFolder(const std::string& base)
{
	mBaseFolder = base;
}


}
