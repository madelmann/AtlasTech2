
#ifndef _Console_Plugins_CLS_h_
#define _Console_Plugins_CLS_h_


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>

// Forward declarations

// Namespace declarations


namespace Console {


class CLS : public IConsolePlugin
{
public:
	CLS()
	: IConsolePlugin("cls")
	{ }

	void execute(Valueizer *) {
		assert(mPrinter);

		mPrinter->print("Clearing screen...");
		mPrinter->clear();
	}

protected:

private:

};


}


#endif
