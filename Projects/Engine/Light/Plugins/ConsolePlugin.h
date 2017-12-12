
#ifndef Light_Plugins_ConsolePlugin_h
#define Light_Plugins_ConsolePlugin_h


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Light/Settings.h>
#include <Light/Types.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations
using namespace Tools;


namespace LightSpace {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("light"),
	  mSettings(0)
	{ }

public:
	void connectSettings(Settings *s) {
		mSettings = s;
	}

	void execute(Valueizer *t) {
		assert(mSettings);
		assert(mPrinter);

		if ( t->hasNext() ) {
			Value val = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( val.ofValue("ambient") ) {
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

								mSettings->ambient(color);

								mPrinter->print("ambient set to " + val.toString() + ".");
							}
						}
					}
				}
				else if ( val.ofValue("attenuation") ) {
					val = t->getTokenAsValue();

					vector3f vec = Evaluator::toVector3f(t);
					mSettings->attenuation(vec);

					mPrinter->print("attenuation set to " + val.toString() + ".");
				}
				else if ( val.ofValue("diffuse") ) {
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

								mSettings->diffuse(color);

								mPrinter->print("diffuse set to " + val.toString() + ".");
							}
						}
					}
				}
				else if ( val.ofValue("direction") ) {
					val = t->getTokenAsValue();

					vector3f vec;
					vec.x = val.toFloat();
					if ( t->hasNext() ) {
						val = Value(t->getToken());
						vec.y = val.toFloat();

						if ( t->hasNext() ) {
							val = Value(t->getToken());
							vec.z = val.toFloat();

							mSettings->direction(vec);

							mPrinter->print("direction set to " + val.toString() + ".");
						}
					}
				}
				else if ( val.ofValue("enabled") ) {
					val = t->getTokenAsValue();

					mSettings->enabled(val.toBool());

					mPrinter->print("enabled set to " + val.toString() + ".");
				}
				else if ( val.ofValue("lightmodel.globalambient") ) {
					if ( !t->hasNext() ) {
						mPrinter->print(toString(mSettings->lightModel().globalAmbient()));
						return;
					}

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

								mSettings->lightModel().globalAmbient(color);

								mPrinter->print("lightmodel.globalambient set to " + toString(mSettings->lightModel().globalAmbient()) + ".");
							}
						}
					}
				}
				else if ( val.ofValue("lightmodel.localviewer") ) {
					if ( !t->hasNext() ) {
						mPrinter->print(toString(mSettings->lightModel().localViewer()));
						return;
					}

					val = t->getTokenAsValue();

					mSettings->lightModel().localViewer(val.toBool());

					mPrinter->print("lightmodel.localviewer set to " + toString(mSettings->lightModel().localViewer()) + ".");
				}
				else if ( val.ofValue("lightmodel.twosided") ) {
					if ( !t->hasNext() ) {
						mPrinter->print(toString(mSettings->lightModel().twoSided()));
						return;
					}

					val = t->getTokenAsValue();

					mSettings->lightModel().twoSided(val.toBool());

					mPrinter->print("lightmodel.twosided set to " + toString(mSettings->lightModel().twoSided()) + ".");
				}
				else if ( val.ofValue("mode") ) {
					val = t->getTokenAsValue();

					mSettings->mode(LightType::convert(val.toInt()));

					mPrinter->print("mode set to " + val.toString() + ".");
				}
				else if ( val.ofValue("position") ) {
					val = t->getTokenAsValue();

					vector3f vec;
					vec.x = val.toFloat();
					if ( t->hasNext() ) {
						val = Value(t->getToken());
						vec.y = val.toFloat();

						if ( t->hasNext() ) {
							val = Value(t->getToken());
							vec.z = val.toFloat();

							mSettings->position(vec);

							mPrinter->print("position set to " + val.toString() + ".");
						}
					}
				}
				else if ( val.ofValue("specular") ) {
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

								mSettings->specular(color);

								mPrinter->print("specular set to " + val.toString() + ".");
							}
						}
					}
				}
			}
			else {
				if ( val.ofValue("ambient") ) {
					mPrinter->print(toString(mSettings->ambient()));
				}
				else if ( val.ofValue("attenuation") ) {
					mPrinter->print(toString(mSettings->attenuation()));
				}
				else if ( val.ofValue("diffuse") ) {
					mPrinter->print(toString(mSettings->diffuse()));
				}
				else if ( val.ofValue("direction") ) {
					mPrinter->print(toString(mSettings->direction()));
				}
				else if ( val.ofValue("enabled") ) {
					mPrinter->print(toString(mSettings->enabled()));
				}
				else if ( val.ofValue("help") ) {
					help();
				}
				else if ( val.ofValue("lightmodel") ) {
					mPrinter->print("lightmodel.globalambient: " + toString(mSettings->lightModel().globalAmbient()));
					mPrinter->print("lightmodel.localviewer: " + toString(mSettings->lightModel().localViewer()));
					mPrinter->print("lightmodel.twosided: " + toString(mSettings->lightModel().twoSided()));
				}
				else if ( val.ofValue("lightmodel.globalambient") ) {
					mPrinter->print(toString(mSettings->lightModel().globalAmbient()));
				}
				else if ( val.ofValue("lightmodel.localviewer") ) {
					mPrinter->print(toString(mSettings->lightModel().localViewer()));
				}
				else if ( val.ofValue("lightmodel.twosided") ) {
					mPrinter->print(toString(mSettings->lightModel().twoSided()));
				}
				else if ( val.ofValue("mode") ) {
					mPrinter->print(toString(mSettings->mode()));
				}
				else if ( val.ofValue("position") ) {
					mPrinter->print(toString(mSettings->position()));
				}
				else if ( val.ofValue("specular") ) {
					mPrinter->print(toString(mSettings->specular()));
				}
			}

			return;
		}

		help();
	}

private:
	void help() {
		mPrinter->print("Usage: " + name() + " <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   ambient <vector4f>");
		mPrinter->print("   attenuation <vector3f>");
		mPrinter->print("   diffuse <vector4f>");
		mPrinter->print("   direction <vector3f>");
		mPrinter->print("   enabled <bool>");
		mPrinter->print("   lightmodel <LightModel>");
		mPrinter->print("   mode <int>");
		mPrinter->print("   position <vector3f>");
		mPrinter->print("   specular <vector4f>");
	}

private:
	Settings	*mSettings;
};


}


#endif
