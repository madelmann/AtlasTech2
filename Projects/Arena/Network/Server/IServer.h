
#ifndef Arena_Network_Server_IServer_h
#define Arena_Network_Server_IServer_h


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Network {
namespace Server {


class IServer
{
public:
	virtual ~IServer() { }

	virtual void listen() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
};


}
}


#endif
