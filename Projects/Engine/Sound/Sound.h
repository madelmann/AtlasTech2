
#ifndef _Sound_Sound_h_
#define _Sound_Sound_h_


// Library includes

// Project includes
#include "BaseSound.h"

// Forward declarations

// Namespace declarations


namespace SoundSpace {


class Sound : public BaseSound
{
public:
	Sound(Core::AEid id = Core::invalidID);
	virtual ~Sound() { }

protected:

private:

};


}


#endif
