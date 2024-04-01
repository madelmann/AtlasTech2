
#ifndef _Input_KeyGrabber_h_
#define _Input_KeyGrabber_h_


// Library includes

// Project includes
#include <Interfaces/Common/AProcess.h>
#include <Interfaces/Input/IKeyboard.h>

// Forward declarations

// Namespace declarations


namespace Input {


class KeyGrabber : public Common::AProcess
{
public:
	KeyGrabber();

	void bind(Key *k);

	void handleEvent(SDL_Event* e);
	void run();

protected:

private:
	Key	*mKey;
};


}


#endif
