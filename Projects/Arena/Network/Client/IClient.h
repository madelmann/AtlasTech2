
#ifndef Arena_Network_Client_IClient_h
#define Arena_Network_Client_IClient_h


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Network {
namespace Client {


class IClient
{
public:
	virtual ~IClient() { }

	virtual bool connect() = 0;
	virtual void disconnect() = 0;
	virtual bool isConnected() const = 0;
};


}
}


#endif
