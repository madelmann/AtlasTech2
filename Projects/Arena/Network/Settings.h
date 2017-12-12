
#ifndef Arena_Network_Settings_h
#define Arena_Network_Settings_h


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace Network {


class Settings
{
public:
	Settings() {
		mEnabled = false;
		mServerAddress[0] = 127;
		mServerAddress[1] = 0;
		mServerAddress[2] = 0;
		mServerAddress[3] = 1;
		mServerPort = 24000;
		mServerIP = "127.0.0.1";
	}

public:
	bool mEnabled;
	int mServerAddress[4];
	unsigned short mServerPort;
	std::string mServerIP;
};


}


#endif
