
#ifndef _Wind_Wind_h_
#define _Wind_Wind_h_


// Library includes

// Project includes
#include "Settings.h"
#include <Common/Logger.h>

// Forward declarations
namespace Console {
	class IConsoleInterface;
}

// Namespace declarations
using Console::IConsoleInterface;


namespace WindSpace {


class Wind : public Common::Logger
{
public:
	Wind(const Common::Logger *p);
	virtual ~Wind();

public:
	// Connectors
	//{
	void connectConsole(IConsoleInterface *c);
	// }

	// Providers
	//{
	Settings& provideSettings();
	//}

	// {
	void clear();
	void init();
	void manage();
	void reset();
	void update(float elapsedTime = 0.f);
	// }

private:
	IConsoleInterface *mConsole;
	Settings mSettings;
};


}


#endif
