
#ifndef Arena_Network_ISender_h
#define Arena_Network_ISender_h


// Project includes
#include "ATelegram.h"


namespace Network {


class ISender
{
public:
	virtual ~ISender() { }

	virtual void send(ATelegram *telegram) = 0;
};


}


#endif
