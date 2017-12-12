
#ifndef Arena_Network_TCPServer_h
#define Arena_Network_TCPServer_h


// Library includes
#include <map>
#include <RakNet/include/RakPeerInterface.h>
#include <RakNet/include/RakNetTypes.h>

// Project includes
#include <Common/Logger.h>
#include <Core/IdGenerator.h>
#include <Network/Client/Telegrams.h>
#include <Network/ATelegram.h>
#include <Network/Settings.h>
#include "IServer.h"

// Forward declarations
class IEntityManager;

// Namespace declarations


namespace Network {
namespace Server {

// Consts
#define MAX_CLIENTS 32


class Client
{
public:
	Client()
	{}

	Client(RakNet::SystemAddress address, Core::AEid id)
	: mAddress(address),
	  mId(id)
	{ }

	const RakNet::SystemAddress& getAddress() const {
		return mAddress;
	}

	const Core::AEid getId() const {
		return mId;
	}

private:
	RakNet::SystemAddress mAddress;
	Core::AEid mId;
};


class TCPServer : public IServer,
				  private Common::Logger
{
public:
	TCPServer(const Common::Logger *p);
	virtual ~TCPServer();

public:	// Setup
	void connectEntityManager(IEntityManager *m);
	void connectIdGenerator(Core::IdGenerator *g);

public: // IServer implementation
	void listen();
	void start();
	void stop();

public: // Public methods
	void init();
	void shutdown();

protected:

private:
	void loadConfig();

	void disconnect(RakNet::SystemAddress address);
	void send(ATelegram *telegram, RakNet::SystemAddress address);

private:
	typedef std::map<RakNet::SystemAddress, Client> ClientMap;

private:
	void updateClientPosition(RakNet::SystemAddress address, const ::Network::Client::ClientPositionUpdate& telegram);
	void updateClientRotation(RakNet::SystemAddress address, const ::Network::Client::ClientRotationUpdate& telegram);

private:
	ClientMap mClients;
	RakNet::RakPeerInterface *mPeer;
	Settings mSettings;

	IEntityManager *mEntityManager;
	Core::IdGenerator *mIdGenerator;
};


}
}


#endif
