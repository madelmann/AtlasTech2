
// Header
#include "TCPServer.h"

// Library includes
#include <RakNet/include/MessageIdentifiers.h>

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Entity/Entity.h>
#include <Entity/EntityManager.h>
#include <Math/Vector3.h>
#include <Network/Client/Telegrams.h>
#include <Network/Server/Telegrams.h>

// Namespace declarations


namespace Network {
namespace Server {


TCPServer::TCPServer(const Common::Logger *p)
: Common::Logger(p, "TCPServer"),
  mEntityManager(0),
  mIdGenerator(0),
  mPeer(RakNet::RakPeerInterface::GetInstance())
{
}

TCPServer::~TCPServer()
{
	shutdown();
}

void TCPServer::connectEntityManager(IEntityManager *m)
{
	mEntityManager = m;
}

void TCPServer::connectIdGenerator(Core::IdGenerator *g)
{
	mIdGenerator = g;
}

void TCPServer::disconnect(RakNet::SystemAddress address)
{
	// a client disconnected => remove his entity
	mEntityManager->remove(mClients[address].getId());
}

void TCPServer::init()
{
	loadConfig();

	if ( mSettings.mEnabled ) {
		start();
	}
}

void TCPServer::listen()
{
	if ( !mSettings.mEnabled || !mPeer ) {
		return;
	}

	RakNet::Packet *packet = mPeer->Receive();
	while ( packet ) {	// Stay in the loop as long as there are more packets
		RakNet::BitStream stream(packet->data, packet->length, false);
		stream.IgnoreBytes(sizeof(RakNet::MessageID));

		switch ( packet->data[0] ) {
			case ID_CONNECTION_LOST:
				// Reliable packets cannot be delivered to the system specified in Packet::systemAddress.  The connection to that system has been closed. 
				disconnect(packet->systemAddress);
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				// The system specified in Packet::systemAddress has disconnected from us.  For the client, this would mean the server has shutdown. 
				disconnect(packet->systemAddress);
				break;
			case ID_NEW_INCOMING_CONNECTION: {
				// A remote system has successfully connected

				// TODO: we have to store this client in our clients list
				LocalPlayerIdTelegram telegram;
				telegram.mId = mIdGenerator->generate();

				// store client address
				mClients.insert(
					std::make_pair(packet->systemAddress, Client(packet->systemAddress, telegram.mId))
				);

				send(&telegram, packet->systemAddress);

				mEntityManager->add(telegram.mId);
				} break;
			case ID_USER_PACKET_ENUM:
				// here we start
				break;
			case ::Network::Client::ToServer::PLAYER_UPDATE_DIRECTION: {
				// a client sent as his view update
				::Network::Client::ClientRotationUpdate telegram(stream);

				updateClientRotation(packet->systemAddress, telegram);
			} break;
			case ::Network::Client::ToServer::PLAYER_UPDATE_POSITION: {
				// a client sent as his position update
				::Network::Client::ClientPositionUpdate telegram(stream);

				updateClientPosition(packet->systemAddress, telegram);
			} break;
			default:
				break;
		}

		mPeer->DeallocatePacket(packet);
		packet = mPeer->Receive();
	}
}

void TCPServer::loadConfig()
{
	Core::Configuration::Configurator config(this, "tcpserver.conf");
	if ( !config.success() ) {
		return;
	}

	mSettings.mEnabled = config.getValue("enabled").toBool();
	mSettings.mServerIP = config.getValue("address").toString();
	mSettings.mServerPort = (unsigned short)config.getValue("port").toInt();
}

void TCPServer::send(ATelegram *telegram, RakNet::SystemAddress address)
{
	if ( !telegram ) {
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

	mPeer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);
	stream.Reset();
}

void TCPServer::start()
{
	if ( mPeer->IsActive() ) {
		// we are already serving
		return;
	}

	loadConfig();

	RakNet::SocketDescriptor sd(mSettings.mServerPort, mSettings.mServerIP.c_str());
	mPeer->Startup(MAX_CLIENTS + 1, &sd, 1);
	mPeer->SetMaximumIncomingConnections(MAX_CLIENTS + 1);
}

void TCPServer::stop()
{
	// disconnect all clients and stop serving
	mPeer->Shutdown(1000);
}

void TCPServer::shutdown()
{
	if ( !mPeer ) {
		return;
	}

	RakNet::RakPeerInterface::DestroyInstance(mPeer);
}

void TCPServer::updateClientPosition(RakNet::SystemAddress address, const ::Network::Client::ClientPositionUpdate& telegram)
{
	static int num = 0;
	//num++;

	Core::AEid clientId = telegram.mClientId;

	if ( num == 30 ) {
		// invalidate client position every 10th time
		::Network::Server::CorrectPlayerPosition correction;
		correction.mDelta = telegram.mDelta;
		correction.mPacketId = telegram.mPacketId;
		correction.mPosition = mEntityManager->getEntity(clientId)->position();

		send(&correction, address);

		num = 0;
		return;
	}

	mEntityManager->updatePosition(clientId, telegram.mPosition);
}

void TCPServer::updateClientRotation(RakNet::SystemAddress /*address*/, const ::Network::Client::ClientRotationUpdate& telegram)
{
	Core::AEid clientId = telegram.mClientId;

	mEntityManager->updateRotation(clientId, telegram.mRotation);
}


}
}
