
#ifndef Arena_Console_Plugins_ClientPlugin_h
#define Arena_Console_Plugins_ClientPlugin_h


// Library includes

// Project includes
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Network/Client/IClient.h>

// Forward declarations

// Namespace declarations


namespace Console {
namespace Plugins {


class ClientPlugin : public IConsolePlugin
{
public:
	ClientPlugin()
	: IConsolePlugin("client"),
	  mClient(0)
	{ }

public:
	void connectClient(Network::Client::IClient *c) {
		mClient = c;
	}

	void execute(Valueizer *t) {
		assert(mClient);
		assert(mPrinter);

		if ( t->hasNext() ) {
			Value value = t->getTokenAsValue();

			if ( t->hasNext() ) {
			}
			else {
				if ( value.ofValue("connect") ) {
					mClient->connect();
				}
				else if ( value.ofValue("disconnect") ) {
					mClient->disconnect();
				}
				else if ( value.ofValue("isconnected") ) {
					mPrinter->print(mClient->isConnected() ? "yes" : "no");
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: client <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   connect");
		mPrinter->print("   disconnect");
		mPrinter->print("   isconnected");
		mPrinter->print("");
	}

private:
	Network::Client::IClient *mClient;
};


}
}


#endif
