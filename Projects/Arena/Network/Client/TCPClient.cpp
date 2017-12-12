
// Header
#include "TCPClient.h"

// Library includes
#include <RakNet/include/MessageIdentifiers.h>

// Project includes
#include <ClientSidePrediction/IPredictionCorrector.h>
#include <ClientSidePrediction/Types.h>
#include <Core/Configuration/Configurator.h>
#include <Entity/EntityManager.h>
#include <Network/Client/Telegrams.h>
#include <Network/Server/Telegrams.h>

// Namespace declarations


namespace Network {
namespace Client {


TCPClient::TCPClient(const Common::Logger *p)
: Common::Logger(p, "TCPClient"),
  mConnectionState(ConnectionState::Initializing),
  mEntityManager(0),
  mLocalPlayerManager(0),
  mPeer(0),
  mPredictionClient(0)
{
}

TCPClient::~TCPClient()
{
	if ( isConnected() ) {
		disconnect();
	}

	if ( mPeer ) {
		delete mPeer;
		mPeer = 0;
	}
}

bool TCPClient::connect()
{
	loadConfig();

	if ( !mSettings.mEnabled ) {
		// just stopp bugging me
		return true;
	}

	mPeer = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor sd;
	mPeer->Startup(1, &sd, 1);

	RakNet::ConnectionAttemptResult result = mPeer->Connect(mSettings.mServerIP.c_str(), mSettings.mServerPort, 0, 0);

	switch ( result ) {
		case RakNet::CONNECTION_ATTEMPT_STARTED: mConnectionState = ConnectionState::Connecting; break;
		case RakNet::INVALID_PARAMETER: mConnectionState = ConnectionState::Failed; break;
		case RakNet::CANNOT_RESOLVE_DOMAIN_NAME: mConnectionState = ConnectionState::Failed; break;
		case RakNet::ALREADY_CONNECTED_TO_ENDPOINT: mConnectionState = ConnectionState::Connected; break;
		case RakNet::CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS: mConnectionState = ConnectionState::Connecting; break;
		case RakNet::SECURITY_INITIALIZATION_FAILED: mConnectionState = ConnectionState::Failed; break;
	}

	//REGISTER_STATIC_RPC(mPeer, &Network::TCPClient::receiveRPC);

	return true;
}

void TCPClient::connectEntityManager(IEntityManager *m)
{
	mEntityManager = m;
}

void TCPClient::connectLocalPlayerManager(ILocalPlayerManager *m)
{
	mLocalPlayerManager = m;
}

void TCPClient::connectPredictionClient(Prediction::IPredictionCorrector *c)
{
	mPredictionClient = c;
}

void TCPClient::disconnect()
{
	mPeer->CloseConnection(mServer, true);

	//RakNet::RakPeerInterface::DestroyInstance(mPeer);

	mConnectionState = ConnectionState::Disconnected;
}

void TCPClient::init()
{
	loadConfig();
}

bool TCPClient::isConnected() const
{
	return (mConnectionState == ConnectionState::Connected);
}

void TCPClient::keepAlive()
{
	if ( !mPeer ) {
		return;
	}

	// send a ping to the server to keep this connection alive
	mPeer->Ping(mServer);
}

void TCPClient::loadConfig()
{
	Core::Configuration::Configurator config(this, "tcpclient.conf");
	if ( !config.success() ) {
		return;
	}

	mSettings.mEnabled = config.getValue("enabled").toBool();
	mSettings.mServerIP = config.getValue("server.address").toString();
	mSettings.mServerPort = (unsigned short)config.getValue("server.port").toInt();
}

void TCPClient::receive()
{
	if ( !mSettings.mEnabled || !mPeer ) {
		return;
	}

	RakNet::Packet *packet = mPeer->Receive();
	while ( packet ) {	// Stay in the loop as long as there are more packets
		RakNet::BitStream stream(packet->data, packet->length, false);
		stream.IgnoreBytes(sizeof(RakNet::MessageID));

		switch ( packet->data[0] ) {
			case ID_ALREADY_CONNECTED: {
				// Sent a connect request to a system we are currently connected to
				mConnectionState = ConnectionState::Connected;
				mServer = packet->systemAddress;
			} break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				// Sent to the player when a connection request cannot be completed due to inability to connect
				disconnect();
				break;
			case ID_CONNECTION_BANNED:
				// We are banned from the system we attempted to connect to
				// TODO: do not try to reconnect
				disconnect();
				break;
			case ID_CONNECTION_LOST:
				// we have lost our connection to the server => close all "open" connections
				disconnect();
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED: {
				// our connection request has been accepted => we are now fully connected
				mConnectionState = ConnectionState::Connected;
				mServer = packet->systemAddress;
			} break;
			case ID_DISCONNECTION_NOTIFICATION:
				// the server has shut down => close all "open" connections
				disconnect();
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// the server is full or cannot accept our connection request
				disconnect();
				break;
			case ID_REMOTE_CONNECTION_LOST:
				// In a client/server environment, a client other than ourselves has been forcefully dropped. Packet::systemAddress is modified to reflect the systemAddress of this client.
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				// ConnectionGraph plugin - In a client/server environment, a client other than ourselves has disconnected gracefully.  Packet::systemAddress is modified to reflect the systemAddress of this client.
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				// In a client/server environment, a client other than ourselves has connected.  Packet::systemAddress is modified to reflect the systemAddress of the client that is not connected directly to us. The packet encoding is SystemAddress 1, ConnectionGraphGroupID 1, SystemAddress 2, ConnectionGraphGroupID 2
				break;
			case ID_USER_PACKET_ENUM:
				// here we start
				break;
			case ::Network::Server::ToClient::PLAYER_ID: {
				// the server tells us our unique id that identifies us through the whole connection
				::Network::Server::LocalPlayerIdTelegram telegram(stream);

				mLocalPlayerManager->initLocalPlayer(telegram.mId, telegram.mInitialPosition);
			} break;
			case ::Network::Server::ToClient::PLAYER_CORRECT_POSITION: {
				// the server corrects the player's position
				::Network::Server::CorrectPlayerPosition telegram(stream);

				Prediction::PredictionPacket p(telegram.mPacketId, telegram.mPosition, telegram.mDelta);
				mPredictionClient->correct(p);
			} break;
			default:
				// we can safely ignore all other messages
				break;
		}

		mPeer->DeallocatePacket(packet);
		packet = mPeer->Receive();
	}
}

void TCPClient::send(ATelegram *telegram)
{
	if ( !isConnected() || !telegram ) {
		// invalid telegram => error
		return;
	}

	/// Sends a block of data to the specified system that you are connected to.
	/// This function only works while the connected
	/// The first byte should be a message identifier starting at ID_USER_PACKET_ENUM
	/// \param[in] data The block of data to send
	/// \param[in] length The size in bytes of the data to send
	/// \param[in] priority What priority level to send on.  See PacketPriority.h
	/// \param[in] reliability How reliability to send this data.  See PacketPriority.h
	/// \param[in] orderingChannel When using ordered or sequenced messages, what channel to order these on. Messages are only ordered relative to other messages on the same stream
	/// \param[in] systemAddress Who to send this packet to, or in the case of broadcasting who not to send it to.  Use UNASSIGNED_SYSTEM_ADDRESS to specify none
	/// \param[in] broadcast True to send this packet to all connected systems. If true, then systemAddress specifies who not to send the packet to.
	/// \return False if we are not connected to the specified recipient.  True otherwise

	RakNet::BitStream stream;
	telegram->serialize(stream);

	mPeer->Send(&stream,
				HIGH_PRIORITY,
				RELIABLE_ORDERED,
				0,
				mServer,
				false
				);
}


}
}
