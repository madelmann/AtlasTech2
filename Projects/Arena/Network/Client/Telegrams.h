
#ifndef Arena_Network_Client_Telegrams_h
#define Arena_Network_Client_Telegrams_h


// Library includes
#include <RakNet/include/MessageIdentifiers.h>

// Project includes
#include <Interfaces/Core/Types.h>
#include <Math/Vector3.h>
#include <Network/ATelegram.h>

// Forward declarations

// Namespace declarations


namespace Network {
namespace Client {


class ToServer
{
public:
	enum E {
		// Client to Server messages
		INIT_VERSION = 220,
		INIT_GAME = 221,

		INIT_PLAYER_NAME = 225,
		INIT_PLAYER_DIRECTION = 226,
		INIT_PLAYER_POSITION = 227,

		PLAYER_UPDATE_DIRECTION = 230,
		PLAYER_UPDATE_POSITION = 231
	};
};


class ClientRotationUpdate : public ATelegram
{
public:
	ClientRotationUpdate()
	: ATelegram(ToServer::PLAYER_UPDATE_DIRECTION),
	  mClientId(0),
	  mPacketId(0)
	{ }

	ClientRotationUpdate(RakNet::BitStream& data)
	: ATelegram(ToServer::PLAYER_UPDATE_DIRECTION),
	  mClientId(0),
	  mPacketId(0)
	{
		unserialize(data);
	}

public:
	void serialize(RakNet::BitStream& data) {
		data.Reset();
		data.Write((RakNet::MessageID)mTelegramId);
		data.Write((int)mClientId);
		data.Write((int)mPacketId);
		data.Write((float)mRotation.x);
		data.Write((float)mRotation.y);
		data.Write((float)mRotation.z);
	}

	void unserialize(RakNet::BitStream& data) {
		data.Read((int)mClientId);
		data.Read((int)mPacketId);
		data.Read((float)mRotation.x);
		data.Read((float)mRotation.y);
		data.Read((float)mRotation.z);
	}

public:
	int mClientId;
	int mPacketId;
	vector3f mRotation;
};


class ClientPositionUpdate : public ATelegram
{
public:
	ClientPositionUpdate()
	: ATelegram(ToServer::PLAYER_UPDATE_POSITION),
	  mClientId(0),
	  mPacketId(0)
	{ }

	ClientPositionUpdate(RakNet::BitStream& data)
	: ATelegram(ToServer::PLAYER_UPDATE_POSITION),
	  mClientId(0),
	  mPacketId(0)
	{
		unserialize(data);
	}

public:
	void serialize(RakNet::BitStream& data) {
		data.Reset();
		data.Write((RakNet::MessageID)mTelegramId);
		data.Write((int)mClientId);
		data.Write((float)mDelta.x);
		data.Write((float)mDelta.y);
		data.Write((float)mDelta.z);
		data.Write((int)mPacketId);
		data.Write((float)mPosition.x);
		data.Write((float)mPosition.y);
		data.Write((float)mPosition.z);
		data.Write((float)mRotation.x);
		data.Write((float)mRotation.y);
		data.Write((float)mRotation.z);
	}

	void unserialize(RakNet::BitStream& data) {
		data.Read((int)mClientId);
		data.Read((float)mDelta.x);
		data.Read((float)mDelta.y);
		data.Read((float)mDelta.z);
		data.Read((int)mPacketId);
		data.Read((float)mPosition.x);
		data.Read((float)mPosition.y);
		data.Read((float)mPosition.z);
		data.Read((float)mRotation.x);
		data.Read((float)mRotation.y);
		data.Read((float)mRotation.z);
	}

public:
	int mClientId;
	vector3f mDelta;
	int mPacketId;
	vector3f mPosition;
	vector3f mRotation;
};


}
}


#endif
