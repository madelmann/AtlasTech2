
#ifndef _Terrain_ConsolePlugin_h_
#define _Terrain_ConsolePlugin_h_


// Library includes

// Project includes
#include "Settings.h"
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations


namespace Terrain {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("terrain"),
	  mSettings(0)
	{ }

	void connectSettings(Settings *s) {
		mSettings = s;
	}

	void execute(Valueizer *t) {
		assert(mPrinter);
		assert(mSettings);

		if ( t->hasNext() ) {
			Value val = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( val.ofValue("debug") ) {
					val = Value(t->getToken());

					mSettings->debugMode(val.toBool());
				}
				else if ( val.ofValue("normals") ) {
					val = Value(t->getToken());

					mSettings->renderNormals(val.toBool());
				}
			}
			else {
				if ( val.ofValue("debug") ) {
					mPrinter->print(Tools::toString(mSettings->debugMode()));
				}
				else if ( val.ofValue("help") ) {
					help();
				}
				else if ( val.ofValue("normals") ) {
					mPrinter->print(Tools::toString(mSettings->renderNormals()));
				}
				else if ( val.ofValue("position") ) {
					mPrinter->print(Tools::toString(mSettings->position()));
				}
				else if ( val.ofValue("size") ) {
					mPrinter->print(Tools::toString(mSettings->size()));
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: terrain <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   debug <bool>");
		mPrinter->print("   normals <bool>");
	}

private:
	Settings	*mSettings;
};


}


#endif
