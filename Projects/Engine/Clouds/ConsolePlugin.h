
#ifndef _Clouds_ConsolePlugin_h_
#define _Clouds_ConsolePlugin_h_


// Library includes

// Project includes
#include "Settings.h"
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>

// Forward declarations

// Namespace declarations


namespace CloudSpace {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("clouds"),
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
				mPrinter->print(name() + ": cannot change values at runtime");
			}
			else {
				if ( val.ofValue("color") ) {
					mPrinter->print(Tools::toString(mSettings->color()));
				}
				else if ( val.ofValue("colormap") ) {
					mPrinter->print(Tools::toString(mSettings->colorMap()));
				}
				else if ( val.ofValue("count") ) {
					mPrinter->print(Tools::toString(mSettings->count()));
				}
				else if ( val.ofValue("enabled") ) {
					mPrinter->print(Tools::toString(mSettings->enabled()));
				}
				else if ( val.ofValue("end") ) {
					mPrinter->print(Tools::toString(mSettings->end()));
				}
				else if ( val.ofValue("height") ) {
					mPrinter->print(Tools::toString(mSettings->height()));
				}
				else if ( val.ofValue("help") ) {
					help();
				}
				else if ( val.ofValue("material") ) {
					mPrinter->print(Tools::toString(mSettings->material()));
				}
				else if ( val.ofValue("scale") ) {
					mPrinter->print(Tools::toString(mSettings->scale()));
				}
				else if ( val.ofValue("size") ) {
					mPrinter->print(Tools::toString(mSettings->sizeMin()) + " to " + Tools::toString(mSettings->sizeMax()));
				}
				else if ( val.ofValue("start") ) {
					mPrinter->print(Tools::toString(mSettings->start()));
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
		mPrinter->print("   color <vector3f>");
		mPrinter->print("   colormap <string>");
		mPrinter->print("   count <int>");
		mPrinter->print("   enabled <bool>");
		mPrinter->print("   end <vector3f>");
		mPrinter->print("   height <float>");
		mPrinter->print("   material <string>");
		mPrinter->print("   scale <vector3f>");
		mPrinter->print("   size <vector3f>");
		mPrinter->print("   start <vector3f>");
	}

private:
	Settings	*mSettings;
};


}


#endif
