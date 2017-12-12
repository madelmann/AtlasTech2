
#ifndef _Physic_ConsolePlugin_h_
#define _Physic_ConsolePlugin_h_


// Library includes

// Project includes
#include "Manager.h"
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations


namespace Physics {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("physics"),
	  mPhysicsManager(0)
	{ }

public:
	void connectPhysicsManager(Manager *w) {
		mPhysicsManager = w;
	}

	void execute(Valueizer *t) {
		assert(mPhysicsManager);
		assert(mPrinter);

		if ( t->hasNext() ) {
			Value val = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( val.ofValue("debug") ) {
					val = Value(t->getToken());

					mPhysicsManager->provideSettings()->debugMode(val.toBool());

					mPrinter->print("debug set to " + val.toString() + ".");
				}
				else if ( val.ofValue("enabled") ) {
					val = Value(t->getToken());

					mPhysicsManager->provideSettings()->enabled(val.toBool());

					mPrinter->print("enabled set to " + val.toString() + ".");
				}
				else if ( val.ofValue("gravity") ) {
					val = Value(t->getToken());

					vector3f value;
					value.x = val.toFloat();
					if ( t->hasNext() ) {
						val = Value(t->getToken());
						value.y = val.toFloat();

						if ( t->hasNext() ) {
							val = Value(t->getToken());
							value.z = val.toFloat();

							mPhysicsManager->provideSettings()->gravity(value);

							mPrinter->print("gravity set to " + Tools::toString(val.toVector3f()) + ".");
						}
					}
				}
			}
			else {
				if ( val.ofValue("boxes") ) {
					mPhysicsManager->debugSetup();
				}
				else if ( val.ofValue("debug") ) {
					mPrinter->print(Tools::toString(mPhysicsManager->provideSettings()->debugMode()));
				}
				else if ( val.ofValue("enabled") ) {
					mPrinter->print(Tools::toString(mPhysicsManager->provideSettings()->enabled()));
				}
				else if ( val.ofValue("gravity") ) {
					mPrinter->print(Tools::toString(mPhysicsManager->provideSettings()->gravity()));
				}
				else if ( val.ofValue("help") ) {
					help();
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: physics <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   debug <bool>");
		mPrinter->print("   enabled <bool>");
		mPrinter->print("   gravity <vector3f>");
		mPrinter->print("Available functions:");
		mPrinter->print("   boxes");
	}

private:
	Manager	*mPhysicsManager;
};


}


#endif
