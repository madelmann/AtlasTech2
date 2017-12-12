
#ifndef _Sky_ConsolePlugin_h_
#define _Sky_ConsolePlugin_h_


// Library includes

// Project includes
#include "Settings.h"
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Math/Vector3.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations


namespace SkySpace {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("sky"),
	  mSettings(0)
	{ }

	void connectSettings(Settings *s) {
		mSettings = s;
	}

	void execute(Valueizer *t) {
		assert(mPrinter);
		assert(mSettings);

		if ( t->hasNext() ) {
			Value value = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( value.ofValue("affectedbyfog") ) {
					value = Value(t->getToken());

					mSettings->affectedByFog(value.toBool());
				}
				else if ( value.ofValue("position") ) {
					value = Value(t->getToken());

					vector3f v;
					v.x = value.toFloat();
					if ( t->hasNext() ) {
						value = Value(t->getToken());
						v.y = value.toFloat();

						if ( t->hasNext() ) {
							value = Value(t->getToken());
							v.z = value.toFloat();

							mSettings->position(v);
						}
					}
				}
				else if ( value.ofValue("size") ) {
					value = Value(t->getToken());

					vector3f v;
					v.x = value.toFloat();
					if ( t->hasNext() ) {
						value = Value(t->getToken());
						v.y = value.toFloat();

						if ( t->hasNext() ) {
							value = Value(t->getToken());
							v.z = value.toFloat();

							mSettings->position(v);
						}
					}

				}
				else if ( value.ofValue("theme") ) {
					value = Value(t->getToken());

					mSettings->theme(value.toString());
				}
			}
			else {
				if ( value.ofValue("affectedbyfog") ) {
					mPrinter->print("affectedbyfog: " + Tools::toString(mSettings->affectedByFog()));
				}
				else if ( value.ofValue("position") ) {
					mPrinter->print("position: " + Tools::toString(mSettings->position()));
				}
				else if ( value.ofValue("size") ) {
					mPrinter->print("size: " + Tools::toString(mSettings->size()));
				}
				else if ( value.ofValue("theme") ) {
					mPrinter->print("theme: " + Tools::toString(mSettings->theme()));
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Available settings:");
		mPrinter->print("   affectedbyfog <bool>");
		mPrinter->print("   position <vector3f>");
		mPrinter->print("   size <vector3f>");
		mPrinter->print("   theme <string>");
	}

private:
	Settings	*mSettings;
};


}


#endif
