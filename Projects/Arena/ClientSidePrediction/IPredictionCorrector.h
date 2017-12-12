
#ifndef Arena_ClientSidePrediction_IPredictionCorrector_h
#define Arena_ClientSidePrediction_IPredictionCorrector_h


// Project includes
#include "Types.h"


namespace Prediction {


class IPredictionCorrector
{
public:
	virtual ~IPredictionCorrector() { }

	virtual void correct(const PredictionPacket& packet) = 0;
};


}


#endif
