
#ifndef Core_Common_Viewport_h
#define Core_Common_Viewport_h


// Library includes

// Project includes
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations

namespace Core {


class Viewport
{
public:
	Viewport(AEint left, AEint top, AEint width, AEint height);

public:
	void set();

protected:

private:
	AEint	mHeight;
	AEint	mLeft;
	AEint	mTop;
	AEint	mWidth;
};


}


#endif
