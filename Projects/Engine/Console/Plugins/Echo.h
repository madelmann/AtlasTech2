
#ifndef _Console_Plugins_Echo_h_
#define _Console_Plugins_Echo_h_


// Library includes

// Project includes
#include <Console/Settings.h>
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations


class Echo : public Console::IConsolePlugin
{
public:
	Echo()
	: Console::IConsolePlugin("echo")
	{ }

	void execute(Valueizer *t) {
		assert(mPrinter);

		if ( t->hasNext() ) {
			if ( Tools::StringCompareI(t->getToken(), "on") ) {
				mPrinter->print("Echo turned on");
				mSettings->setEcho(true);
			}
			else if ( Tools::StringCompareI(t->getToken(), "off") ) {
				mPrinter->print("Echo turned off");
				mSettings->setEcho(false);
			}
			else {
				mPrinter->print("error while executing command!");
			}
		}
	}

protected:

private:

};


#endif
