
#ifndef Arena_Network_TCPClient_h
#define Arena_Network_TCPClient_h


// Library includes
#include <RakNet/include/RakPeerInterface.h>
#include <RakNet/include/RakNetTypes.h>
#include <RakNet/include/BitStream.h>

// Project includes
#include <Common/Logger.h>
#include <ClientSidePrediction/IPredictionCorrector.h>
#include <Network/ATelegram.h>
#include <Network/ILocalPlayerManager.h>
#include <Network/ISender.h>
#include <Network/Settings.h>
#include "IClient.h"

// Forward declarations
class IEntityManager;
namespace Prediction {
	class IPredictionCorrector;
}

// Namespace declarations


namespace Network {
namespace Client {


class TCPClient : public IClient,
				  public ISender,
				  private Common::Logger
{
public:
	class ConnectionState {
	public:
		enum E {
			Initializing = 0,
			Connecting,
			Connected,
			Disconnected,
			Failed
		};
	};

public:
	TCPClient(const Common::Logger *p);
	virtual ~TCPClient();

public: // Setup
	void connectEntityManager(IEntityManager *m);
	void connectLocalPlayerManager(ILocalPlayerManager *m);
	void connectPredictionClient(Prediction::IPredictionCorrector *c);

public:	// IClient implementation
	bool connect();
	void disconnect();
	bool isConnected() const;

public:
	void init();
	void keepAlive();
	void receive();
	void send(ATelegram *telegram);

protected:

private:
	void loadConfig();
	void processPacket(RakNet::Packet *packet);

private:
	ConnectionState::E mConnectionState;
	RakNet::RakPeerInterface *mPeer;
	RakNet::SystemAddress mServer;
	Settings mSettings;

	IEntityManager *mEntityManager;
	ILocalPlayerManager *mLocalPlayerManager;
	Prediction::IPredictionCorrector *mPredictionClient;
};


}
}


#endif
