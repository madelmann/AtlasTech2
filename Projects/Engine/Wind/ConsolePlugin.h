
#ifndef _Wind_ConsolePlugin_h_
#define _Wind_ConsolePlugin_h_


// Library includes

// Project includes
#include "Wind.h"
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Math/Vector3.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations


namespace WindSpace {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("wind"),
	  mWind(0)
	{ }

public:
	void connectWind(Wind *w) {
		mWind = w;
	}

	void execute(Valueizer *t) {
		assert(mPrinter);
		assert(mWind);

		if ( t->hasNext() ) {
			Value val = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( val.ofValue("direction") ) {
					val = Value(t->getToken());

					vector3f dir;
					dir.x = val.toFloat();
					if ( t->hasNext() ) {
						val = Value(t->getToken());
						dir.y = val.toFloat();

						if ( t->hasNext() ) {
							val = Value(t->getToken());
							dir.z = val.toFloat();

							mWind->provideSettings().setDirection(dir);

							mPrinter->print("direction set to " + Tools::toString(val.toVector4f()) + ".");
						}
					}
				}
			}
			else {
				if ( val.ofValue("direction") ) {
					mPrinter->print(Tools::toString(mWind->provideSettings().getDirection()));
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: wind <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   direction <vector3f>");
	}

private:
	Wind	*mWind;
};


}


#endif
