
#ifndef _Console_Plugins_EditorPlugin_h_
#define _Console_Plugins_EditorPlugin_h_


// Library includes

// Project includes
#include <Interfaces\Console\IConsolePlugin.h>
#include <Interfaces\Console\IConsolePrinter.h>
#include <Interfaces\Core\ICoreManagement.h>

// Forward declarations

// Namespace declarations


namespace Console {
namespace Plugins {


class EditorPlugin : public IConsolePlugin
{
public:
	EditorPlugin()
	: IConsolePlugin("editor"),
	  mCoreManagement(0)
	{ }

public:
	void connectManagement(Core::ICoreManagement *m) {
		mCoreManagement = m;
	}

	void execute(Valueizer *t) {
		assert(mCoreManagement);
		assert(mPrinter);

		if ( t->hasNext() ) {
			Value value = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( value.ofValue("load") ) {
					value = Value(t->getToken());

					mPrinter->print("loading '" + value.toString() + "':");

					if ( mCoreManagement->loadScene(value.toString()) ) {
						mPrinter->print("done.");
					}
					else {
						mPrinter->print("failed!");
					}
				}
			}
			else {
				if ( value.ofValue("getscenename") ) {
					mPrinter->print("Scene: '" + mCoreManagement->getSceneName() +  "'");
				}
				else if ( value.ofValue("unload") ) {
					mPrinter->print("unloading scene:");

					if ( mCoreManagement->unloadScene() ) {
						mPrinter->print("successful");
					}
					else {
						mPrinter->print("failed");
					}
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: editor <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   getscenename");
		mPrinter->print("   load <string>");
		mPrinter->print("   unload");
		mPrinter->print("");
	}

private:
	Core::ICoreManagement	*mCoreManagement;
};


}
}


#endif
