
#ifndef Arena_ClientSidePrediction_ClientSidePrediction_h
#define Arena_ClientSidePrediction_ClientSidePrediction_h


// Library includes
#include <map>

// Project includes
#include <Math/Vector3.h>
#include <Network/ISender.h>
#include <Network/Client/Telegrams.h>
#include <Network/Server/Telegrams.h>
#include "IPredictionCorrector.h"

// Forward declarations

// Namespace declarations


namespace Prediction {


class PacketFactory
{
public:
	PacketFactory()
	: mPacketId(1)
	{ }

	PredictionPacket newPacket(const vector3f& position, const vector3f& delta) {
		if ( mPacketId >= SIZE_MAX - 1 ) {
			mPacketId = 1;
		}

		return PredictionPacket(mPacketId++, position, delta);
	}

private:
	unsigned int mPacketId;
};

class ClientSidePrediction : public IPredictionCorrector
{
private:
	typedef std::map<PacketId, PredictionPacket> Packets;

	static const unsigned int NumOfHoldPackets = 10;

public:
	ClientSidePrediction();
	~ClientSidePrediction();

public:	// Setup
	void connectSender(Network::ISender *s);

	void setClientId(int clientId) {
		mClientId = clientId;
	}

public:
	void correct(const PredictionPacket& packet);
	void removeOldData(const PredictionPacket& beforePacket = PredictionPacket());
	vector3f update(const vector3f& position, const vector3f& rotation);

private:
	int mClientId;
	bool mCorrected;
	vector3f mLastPosition;
	vector3f mLastRotation;
	PacketFactory mPacketFactory;
	Packets mPackets;
	Network::ISender *mSender;
};


}


#endif
