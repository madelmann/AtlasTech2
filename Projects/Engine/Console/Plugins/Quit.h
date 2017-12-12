
#ifndef _Console_Plugins_Quit_h_
#define _Console_Plugins_Quit_h_


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>

// Forward declarations

// Namespace declarations


class Quit : public Console::IConsolePlugin
{
public:
	Quit()
	: Console::IConsolePlugin("quit")
	{ }

	void execute(Valueizer *) {
		mPrinter->print("Quitting...");

		PostQuitMessage(0);													// Tell windows we want to quit
	}

protected:

private:

};


#endif
