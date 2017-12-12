
#ifndef _Console_Plugins_Help_h_
#define _Console_Plugins_Help_h_


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Tools/Tools.h>

// Forward declarations

// Namespace declarations


class Help : public Console::IConsolePlugin
{
public:
	Help()
	: Console::IConsolePlugin("help")
	{ }

	void connectMediaPathManager(IMediaPathManager *m)
	{
		mMediaPathManager = m;
	}

	void execute(Valueizer *) {
		assert(mMediaPathManager);
		assert(mPrinter);

		mPrinter->print("");
		mPrinter->print("Available commands:");

		// Load autocomplete commands from file
		FILE *pFile;
		std::string filename = mMediaPathManager->getGame()
							 + mMediaPathManager->getSystem()
							 + mMediaPathManager->getConsole()
							 + "commands.txt";

		fopen_s(&pFile, filename.c_str(), "rt");
		if ( !pFile ) {
			mPrinter->print("Could not load help database");
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
		mPrinter->print("To get more details about a command enter 'MAN <Command>'");
		mPrinter->print("");
	}

protected:

private:
	IMediaPathManager	*mMediaPathManager;
};


#endif
