
#ifndef _Console_Plugins_TEMPLATE_h_
#define _Console_Plugins_TEMPLATE_h_


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>

// Forward declarations

// Namespace declarations


class TEMPLATE : public ConsoleSpace::IConsolePlugin
{
public:
	TEMPLATE() {
		mName = "TEMPLATE";
	}

	void execute(Valueizer *) {
		assert(mPrinter);

		mPrinter->print(mName);
	}

protected:

private:

};


#endif
