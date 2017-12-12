
#ifndef Arena_ClientSidePrediction_Types_h
#define Arena_ClientSidePrediction_Types_h


// Project includes
#include <Math/Vector3.h>


namespace Prediction {


typedef unsigned int PacketId;

class PredictionPacket
{
public:
	PredictionPacket()
	: mPacketId(0)
	{ }

	PredictionPacket(PacketId id, const vector3f& pos, const vector3f& delta)
	: mDelta(delta),
	  mPacketId(id),
	  mPosition(pos)
	{ }

public:
	void correct(const vector3f& pos, const vector3f& delta) {
		mDelta = delta;
		mPosition = pos;
	}

	const vector3f& delta() const {
		return mDelta;
	}

	const PacketId packetId() const {
		return mPacketId;
	}

	const vector3f& position() const {
		return mPosition;
	}

private:
	vector3f mDelta;
	PacketId mPacketId;
	vector3f mPosition;
};


}


#endif
