
#ifndef Arena_Console_Plugins_ServerPlugin_h
#define Arena_Console_Plugins_ServerPlugin_h


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Network/Server/IServer.h>

// Forward declarations

// Namespace declarations


namespace Console {
namespace Plugins {


class ServerPlugin : public IConsolePlugin
{
public:
	ServerPlugin()
	: IConsolePlugin("server"),
	  mServer(0)
	{ }

public:
	void connectServer(Network::Server::IServer *s) {
		mServer = s;
	}

	void execute(Valueizer *t) {
		assert(mPrinter);
		assert(mServer);

		if ( t->hasNext() ) {
			Value value = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( value.ofValue("list") ) {
					// list all (half) connected clients
				}
			}
			else {
				if ( value.ofValue("list") ) {
					// list all (half) connected clients
				}
				else if ( value.ofValue("start") ) {
					mServer->start();
				}
				else if ( value.ofValue("stop") ) {
					mServer->stop();
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: server <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   list");
		mPrinter->print("   start");
		mPrinter->print("   stop");
		mPrinter->print("");
	}

private:
	Network::Server::IServer *mServer;
};


}
}


#endif
