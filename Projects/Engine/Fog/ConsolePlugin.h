
#ifndef Fog_ConsolePlugin_h
#define Fog_ConsolePlugin_h


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
using Tools::toString;


namespace FogSpace {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("fog"),
	  mSettings(0)
	{ }

public:
	void connectSettings(Settings *s) {
		mSettings = s;
	}

	void execute(Valueizer *t) {
		assert(mPrinter);
		assert(mSettings);

		if ( t->hasNext() ) {
			Value val = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( val.ofValue("affectsskybox") ) {
					val = t->getTokenAsValue();

					mSettings->affectsSkyBox(val.toBool());

					mPrinter->print("affectsskybox set to " + val.toString() + ".");
				}
				else if ( val.ofValue("color") ) {
					val = t->getTokenAsValue();

					vector4f color;
					color.x = val.toFloat();
					if ( t->hasNext() ) {
						val = Value(t->getToken());
						color.y = val.toFloat();

						if ( t->hasNext() ) {
							val = Value(t->getToken());
							color.z = val.toFloat();

							if ( t->hasNext() ) {
								val = Value(t->getToken());
								color.w = val.toFloat();

								mSettings->color(color);

								mPrinter->print("color set to " + toString(mSettings->color()) + ".");
							}
						}
					}
				}
				else if ( val.ofValue("density") ) {
					val = t->getTokenAsValue();

					mSettings->density(val.toFloat());

					mPrinter->print("density set to " + toString(mSettings->density()) + ".");
				}
				else if ( val.ofValue("enabled") ) {
					val = t->getTokenAsValue();

					mSettings->enabled(val.toBool());

					mPrinter->print("enabled set to " + toString(mSettings->enabled()) + ".");
				}
				else if ( val.ofValue("end") ) {
					val = Value(t->getToken());

					mSettings->end(val.toFloat());

					mPrinter->print("end set to " + toString(mSettings->end()) + ".");
				}
				else if ( val.ofValue("mode") ) {
					val = t->getTokenAsValue();

					mSettings->mode(FogMode::convert(val.toInt()));

					mPrinter->print("mode set to " + toString(mSettings->mode()) + ".");
				}
				else if ( val.ofValue("start") ) {
					val = t->getTokenAsValue();

					mSettings->start(val.toFloat());

					mPrinter->print("start set to " + toString(mSettings->start()) + ".");
				}
			}
			else {
				if ( val.ofValue("affectsskybox") ) {
					mPrinter->print(toString(mSettings->affectsSkyBox()));
				}
				else if ( val.ofValue("color") ) {
					mPrinter->print(toString(mSettings->color()));
				}
				else if ( val.ofValue("density") ) {
					mPrinter->print(toString(mSettings->density()));
				}
				else if ( val.ofValue("end") ) {
					mPrinter->print(toString(mSettings->end()));
				}
				else if ( val.ofValue("enabled") ) {
					mPrinter->print(toString(mSettings->enabled()));
				}
				else if ( val.ofValue("help") ) {
					help();
				}
				else if ( val.ofValue("mode") ) {
					mPrinter->print(toString(mSettings->mode()));
				}
				else if ( val.ofValue("start") ) {
					mPrinter->print(toString(mSettings->start()));
				}
				else {
					mPrinter->print(name() + ": error while executing command '" + val.toString() + "'!");
				}
			}

			// return after all parameters have been processed
			return;
		}

		help();
	}

private:
	void help() {
		mPrinter->print("Available settings:");
		mPrinter->print("   affectsskybox <bool>");
		mPrinter->print("   color <vector3f>");
		mPrinter->print("   density <float>");
		mPrinter->print("   enabled <bool>");
		mPrinter->print("   end <float>");
		mPrinter->print("   mode <int>");
		mPrinter->print("   start <float>");
	}

private:
	Settings	*mSettings;
};


}


#endif
