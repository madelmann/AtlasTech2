
#ifndef Water_Plugins_ConsolePlugin_h
#define Water_Plugins_ConsolePlugin_h


// Library includes

// Project includes
#include "Settings.h"
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations


namespace WaterSpace {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("water"),
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
				if ( val.ofValue("color") ) {
					val = Value(t->getToken());

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
							}
							else {
								color.w = 1.f;
							}

							mSettings->color(color);

							mPrinter->print("color set to " + Tools::toString(val.toVector4f()) + ".");
						}
					}
				}
				else if ( val.ofValue("enabled") ) {
					val = Value(t->getToken());

					mSettings->enabled(val.toBool());

					mPrinter->print("enabled set to " + val.toString() + ".");
				}
				else if ( val.ofValue("facing") ) {
					val = Value(t->getToken());

					if ( val.ofValue("both") || val.ofValue("0") ) {
						mSettings->facing(Core::Facing::Both);
					}
					else if ( val.ofValue("back") || val.ofValue("1") ) {
						mSettings->facing(Core::Facing::Back);
					}
					else if ( val.ofValue("front") || val.ofValue("2") ) {
						mSettings->facing(Core::Facing::Front);
					}
					else {
						mPrinter->print("invalid parameter provided!");
					}

					mPrinter->print("facing set to " + val.toString() + ".");
				}
				else if ( val.ofValue("flowspeed") ) {
					val = Value(t->getToken());

					mSettings->flowSpeed(val.toVector2f());

					mPrinter->print("flowspeed set to " + val.toString() + ".");
				}
				else if ( val.ofValue("offset") ) {
					val = Value(t->getToken());

					vector3f pos = mSettings->position();

					mSettings->position(
						vector3f(pos.x, val.toFloat(), pos.z)
					);

					mPrinter->print("offset set to " + Tools::toString(mSettings->position().y) + ".");
				}
				else if ( val.ofValue("position") ) {
					val = Value(t->getToken());

					vector3f pos;
					pos.x = val.toFloat();
					if ( t->hasNext() ) {
						val = Value(t->getToken());
						pos.y = val.toFloat();

						if ( t->hasNext() ) {
							val = Value(t->getToken());
							pos.z = val.toFloat();

							mSettings->position(pos);

							mPrinter->print("position set to " + Tools::toString(mSettings->position()) + ".");
						}
					}
				}
				else if ( val.ofValue("reflections") ) {
					val = Value(t->getToken());

					mSettings->useReflections(val.toBool());

					mPrinter->print("reflections set to " + val.toString() + ".");
				}
				else if ( val.ofValue("refractions") ) {
					val = Value(t->getToken());

					mSettings->useRefractions(val.toBool());

					mPrinter->print("refractions set to " + val.toString() + ".");
				}
				else if ( val.ofValue("size") ) {
					val = Value(t->getToken());

					vector3f size;
					size.x = val.toFloat();
					if ( t->hasNext() ) {
						val = Value(t->getToken());
						size.y = val.toFloat();

						if ( t->hasNext() ) {
							val = Value(t->getToken());
							size.z = val.toFloat();

							mSettings->size(size);

							mPrinter->print("size set to " + Tools::toString(mSettings->size()) + ".");
						}
					}
				}
				else if ( val.ofValue("waveheight") ) {
					val = Value(t->getToken());

					mSettings->waveHeight(val.toFloat());

					mPrinter->print("waveheight set to " + val.toString() + ".");
				}
				else if ( val.ofValue("waveinterval") ) {
					val = Value(t->getToken());

					mSettings->waveInterval(val.toFloat());

					mPrinter->print("waveinterval set to " + val.toString() + ".");
				}
			}
			else {
				if ( val.ofValue("color") ) {
					mPrinter->print(Tools::toString(mSettings->color()));
				}
				else if ( val.ofValue("enabled") ) {
					mPrinter->print(Tools::toString(mSettings->enabled()));
				}
				else if ( val.ofValue("facing") ) {
					mPrinter->print(Tools::toString(mSettings->facing()));
				}
				else if ( val.ofValue("flowspeed") ) {
					mPrinter->print(Tools::toString(mSettings->flowSpeed()));
				}
				else if ( val.ofValue("help") ) {
					help();
				}
				else if ( val.ofValue("offset") ) {
					mPrinter->print(Tools::toString(mSettings->position().y));
				}
				else if ( val.ofValue("position") ) {
					mPrinter->print(Tools::toString(mSettings->position()));
				}
				else if ( val.ofValue("size") ) {
					mPrinter->print(Tools::toString(mSettings->size()));
				}
				else if ( val.ofValue("waveheight") ) {
					mPrinter->print(Tools::toString(mSettings->waveHeight()));
				}
				else if ( val.ofValue("waveinterval") ) {
					mPrinter->print(Tools::toString(mSettings->waveInterval()));
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: water <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   color <vector3f>");
		mPrinter->print("   enabled <bool>");
		mPrinter->print("   facing <both/down/up>");
		mPrinter->print("   flowspeed <float>");
		mPrinter->print("   offset <float>");
		mPrinter->print("   position <vector3f>");
		mPrinter->print("   reflections <bool>");
		mPrinter->print("   refractions <bool>");
		mPrinter->print("   size <vector3f>");
		mPrinter->print("   waveheight <float>");
		mPrinter->print("   waveinterval <float>");
	}

private:
	Settings	*mSettings;
};


}


#endif
