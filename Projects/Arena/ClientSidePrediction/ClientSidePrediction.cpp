
// Header
#include "ClientSidePrediction.h"

// Library includes

// Project includes

// Namespace declarations


namespace Prediction {


ClientSidePrediction::ClientSidePrediction()
: mCorrected(false),
  mClientId(0),
  mSender(0)
{
}

ClientSidePrediction::~ClientSidePrediction()
{
}

void ClientSidePrediction::connectSender(Network::ISender *s)
{
	mSender = s;
}

void ClientSidePrediction::correct(const PredictionPacket& packet)
{
	Packets::iterator it = mPackets.find(packet.packetId());
	if ( it == mPackets.end() ) {
		// we should correct a packet that we don't have (anymore) => error
		return;
	}

	// replace old packet with our new (corrected) one
	it->second.correct(packet.position(), packet.delta());

	// and remove all preceding packets
	removeOldData(packet);

	mCorrected = true;
}

void ClientSidePrediction::removeOldData(const PredictionPacket& beforePacket)
{
	if ( mPackets.empty() || mPackets.size() < NumOfHoldPackets ) {
		return;
	}

	Packets::const_iterator last;
	if ( beforePacket.packetId() == 0 ) {
		last = mPackets.find(mPackets.begin()->first + (mPackets.size() - NumOfHoldPackets));
	}
	else {
		last = mPackets.find(beforePacket.packetId());
	}

	mPackets.erase(
		mPackets.begin(),
		last
	);
}

vector3f ClientSidePrediction::update(const vector3f& position, const vector3f& rotation)
{
	PredictionPacket packet = mPacketFactory.newPacket(position, mLastPosition - position);
	mPackets[packet.packetId()] = packet;

	if ( position != mLastPosition ) {
		Network::Client::ClientPositionUpdate posUpdate;
		posUpdate.mClientId = mClientId;
		posUpdate.mDelta = packet.delta();
		posUpdate.mPacketId = packet.packetId();
		posUpdate.mPosition = packet.position();
		mSender->send(&posUpdate);
	}

	if ( rotation != mLastRotation ) {
		Network::Client::ClientRotationUpdate rotUpdate;
		rotUpdate.mClientId = mClientId;
		rotUpdate.mPacketId = packet.packetId();	// this can not be corrected, so we don't need a unique packet id
		rotUpdate.mRotation = rotation;
		mSender->send(&rotUpdate);
	}

	mLastPosition = position;

	if ( mCorrected ) {
		mCorrected = false;

		for ( Packets::const_iterator it = mPackets.begin(); it != mPackets.end(); ++it ) {
			if ( it == mPackets.begin() ) {
				// set initial position for first packet
				mLastPosition = it->second.position();
				continue;
			}

			mLastPosition += it->second.delta();
		}
	}

	return mLastPosition;
}


}
