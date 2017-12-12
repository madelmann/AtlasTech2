
#ifndef _Console_ConsolePlugin_h_
#define _Console_ConsolePlugin_h_


// Library includes

// Project includes
#include "Console.h"
#include "Settings.h"
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations
using Tools::toString;


namespace Console {


class ConsolePlugin : public IConsolePlugin
{
public:
	ConsolePlugin()
	: IConsolePlugin("console"),
	  mConsole(0),
	  mSettings(0)
	{ }

public:
	void connectConsole(Console *c) {
		mConsole = c;
		mSettings = mConsole->provideSettings();
	}

	void execute(Valueizer *t) {
		assert(mConsole);
		assert(mPrinter);

		if ( t->hasNext() ) {
			Value value = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( value.ofValue("fontname") ) {
					value = Value(t->getToken());

					mSettings->setFontName(value.toString());

					mPrinter->print("Console: font name set to " + toString(mSettings->getFontName()) + ".");
				}
				else if ( value.ofValue("fontsize") ) {
					value = Value(t->getToken());

					mSettings->setFontSize(value.toInt());

					mPrinter->print("Console: font size set to " + toString(mSettings->getFontSize()) + ".");
				}
				else if ( value.ofValue("lines") ) {
					value = Value(t->getToken());

					mSettings->setVisibleLines(value.toInt());

					mPrinter->print("Console: visible lines set to " + toString(mSettings->getVisibleLines()) + ".");
				}
				else if ( value.ofValue("linespace") ) {
					value = Value(t->getToken());

					mSettings->setLineSpace(value.toFloat());

					mPrinter->print("Console: line space set to " + toString(mSettings->getLineSpace()) + ".");
				}
			}
			else {
				if ( value.ofValue("fontname") ) {
					mPrinter->print(mSettings->getFontName());
				}
				else if ( value.ofValue("fontsize") ) {
					mPrinter->print(Tools::toString(mSettings->getFontSize()));
				}
				else if ( value.ofValue("help") ) {
					help();
				}
				else if ( value.ofValue("hide") ) {
					mConsole->hide();
				}
				else if ( value.ofValue("hidebackground") ) {
					mSettings->hideBackground();
				}
				else if ( value.ofValue("lines") ) {
					mPrinter->print(Tools::toString(mSettings->getVisibleLines()));
				}
				else if ( value.ofValue("linespace") ) {
					mPrinter->print(Tools::toString(mSettings->getLineSpace()));
				}
				else if ( value.ofValue("showbackground") ) {
					mSettings->showBackground();
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: " + name() + " <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   fontname <string>");
		mPrinter->print("   fontsize <uint>");
		mPrinter->print("   lines <uint>");
		mPrinter->print("   linespace <float>");
		mPrinter->print("Available functions:");
		mPrinter->print("   hide");
		mPrinter->print("   hidebackground");
		mPrinter->print("   showbackground");
		mPrinter->print("");
	}

private:
	Console		*mConsole;
	Settings	*mSettings;
};


}


#endif
