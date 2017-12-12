
#ifndef _Console_Plugins_Man_h_
#define _Console_Plugins_Man_h_


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Tools/Tools.h>

// Forward declarations

// Namespace declarations


class Man : public Console::IConsolePlugin
{
public:
	Man()
	: IConsolePlugin("man")
	{ }

	void connectMediaPathManager(IMediaPathManager *m)
	{
		mMediaPathManager = m;
	}

	void execute(Valueizer *t) {
		assert(mMediaPathManager);
		assert(mPrinter);

		if ( !t->hasNext() ) {
			mPrinter->print("parameter expected - aborting...");
			return;
		}

		mPrinter->print("");

		// Load command description from file
		std::string filename = mMediaPathManager->getGame()
							 + mMediaPathManager->getSystem()
							 + mMediaPathManager->getConsole()
							 + t->getToken() + ".txt";

		FILE *pFile;
		fopen_s(&pFile, filename.c_str(), "rt");
		if ( !pFile ) {
			mPrinter->print("could not load man database for command '" + t->getToken() + "'");
			return;
		}

		char oneline[255];

		do {
			sprintf(oneline, "");
			Tools::readline(pFile, oneline);

			mPrinter->print(oneline);
		} while ( strcmp(oneline, "") != 0 );

		fclose(pFile);

		mPrinter->print("");
	}

protected:

private:
	IMediaPathManager	*mMediaPathManager;
};


#endif
