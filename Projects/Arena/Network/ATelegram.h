
#ifndef Arena_Network_Telegram_h
#define Arena_Network_Telegram_h


// Library includes
#include <RakNet/include/BitStream.h>
#include <RakNet/include/MessageIdentifiers.h>

// Project includes

// Forward declarations

// Namespace declarations


class ATelegram
{
public:
	virtual ~ATelegram() { }

	virtual void serialize(RakNet::BitStream& data) = 0;
	virtual void unserialize(RakNet::BitStream& data) = 0;

protected:
	ATelegram(RakNet::MessageID telegramId = 0)
	: mTelegramId(telegramId)
	{ }

protected:
	RakNet::MessageID mTelegramId;
};


#endif
