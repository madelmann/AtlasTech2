
#ifndef Arena_Network_Server_h
#define Arena_Network_Server_h


// Library includes
#include <RakNet/include/MessageIdentifiers.h>

// Project includes
#include <Interfaces/Core/Types.h>
#include <Math/Vector3.h>
#include <Network/ATelegram.h>


namespace Network {
namespace Server {


class ToClient
{
public:
	enum E {
		// Server to Client messages
		INIT_VERSION = 200,
		INIT_GAME = 201,

		PLAYER_ID = 205,
		PLAYER_CORRECT_POSITION = 206,

		ENTITY_ADD = 211,
		ENTITY_DELETE = 212,
		ENTITY_UPDATE = 213
	};
};


class EntityAdd: public ATelegram
{
public:
	EntityAdd()
	: ATelegram(ToClient::ENTITY_ADD),
	  mId(0)
	{ }

	EntityAdd(RakNet::BitStream& data)
	: ATelegram(ToClient::ENTITY_ADD),
	  mId(0)
	{
		unserialize(data);
	}

public:
	void serialize(RakNet::BitStream& data) {
		data.Reset();
		data.Write((RakNet::MessageID)mTelegramId);
		data.Write((int)mId);
		data.Write((float)mInitialPosition.x);
		data.Write((float)mInitialPosition.y);
		data.Write((float)mInitialPosition.z);
	}

	void unserialize(RakNet::BitStream& data) {
		data.Read((int)mId);
		data.Read((float)mInitialPosition.x);
		data.Read((float)mInitialPosition.y);
		data.Read((float)mInitialPosition.z);
	}

public:
	int mId;
	vector3f mInitialPosition;
};


class EntityDelete: public ATelegram
{
public:
	EntityDelete()
	: ATelegram(ToClient::ENTITY_ADD),
	  mId(0)
	{ }

	EntityDelete(RakNet::BitStream& data)
	: ATelegram(ToClient::ENTITY_ADD),
	  mId(0)
	{
		unserialize(data);
	}

public:
	void serialize(RakNet::BitStream& data) {
		data.Reset();
		data.Write((RakNet::MessageID)mTelegramId);
		data.Write((int)mId);
	}

	void unserialize(RakNet::BitStream& data) {
		data.Read((int)mId);
	}

public:
	int mId;
};


class EntityUpdate: public ATelegram
{
public:
	EntityUpdate()
	: ATelegram(ToClient::ENTITY_ADD),
	  mId(0)
	{ }

	EntityUpdate(RakNet::BitStream& data)
	: ATelegram(ToClient::ENTITY_ADD),
	  mId(0)
	{
		unserialize(data);
	}

public:
	void serialize(RakNet::BitStream& data) {
		data.Reset();
		data.Write((RakNet::MessageID)mTelegramId);
		data.Write((int)mId);
		data.Write((float)mPosition.x);
		data.Write((float)mPosition.y);
		data.Write((float)mPosition.z);
		data.Write((float)mRotation.x);
		data.Write((float)mRotation.y);
		data.Write((float)mRotation.z);
	}

	void unserialize(RakNet::BitStream& data) {
		data.Read((int)mId);
		data.Read((float)mPosition.x);
		data.Read((float)mPosition.y);
		data.Read((float)mPosition.z);
		data.Read((float)mRotation.x);
		data.Read((float)mRotation.y);
		data.Read((float)mRotation.z);
	}

public:
	int mId;
	vector3f mPosition;
	vector3f mRotation;
};


class LocalPlayerIdTelegram : public ATelegram
{
public:
	LocalPlayerIdTelegram()
	: ATelegram(ToClient::PLAYER_ID),
	  mId(0)
	{ }

	LocalPlayerIdTelegram(RakNet::BitStream& data)
	: ATelegram(ToClient::PLAYER_ID),
	  mId(0)
	{
		unserialize(data);
	}

public:
	void serialize(RakNet::BitStream& data) {
		data.Reset();
		data.Write((RakNet::MessageID)mTelegramId);
		data.Write((int)mId);
		data.Write((float)mInitialPosition.x);
		data.Write((float)mInitialPosition.y);
		data.Write((float)mInitialPosition.z);
	}

	void unserialize(RakNet::BitStream& data) {
		data.Read((int)mId);
		data.Read((float)mInitialPosition.x);
		data.Read((float)mInitialPosition.y);
		data.Read((float)mInitialPosition.z);
	}

public:
	Core::AEid mId;
	vector3f mInitialPosition;
};


class CorrectPlayerPosition : public ATelegram
{
public:
	CorrectPlayerPosition()
	: ATelegram(ToClient::PLAYER_CORRECT_POSITION),
	  mPacketId(0)
	{ }

	CorrectPlayerPosition(RakNet::BitStream& data)
	: ATelegram(ToClient::PLAYER_CORRECT_POSITION),
	  mPacketId(0)
	{
		unserialize(data);
	}

public:
	void serialize(RakNet::BitStream& data) {
		data.Reset();
		data.Write((RakNet::MessageID)mTelegramId);
		data.Write((float)mDelta.x);
		data.Write((float)mDelta.y);
		data.Write((float)mDelta.z);
		data.Write((int)mPacketId);
		data.Write((float)mPosition.x);
		data.Write((float)mPosition.y);
		data.Write((float)mPosition.z);
	}

	void unserialize(RakNet::BitStream& data) {
		data.Read((float)mDelta.x);
		data.Read((float)mDelta.y);
		data.Read((float)mDelta.z);
		data.Read((int)mPacketId);
		data.Read((float)mPosition.x);
		data.Read((float)mPosition.y);
		data.Read((float)mPosition.z);
	}

public:
	vector3f mDelta;
	int mPacketId;
	vector3f mPosition;
};


}
}


#endif
