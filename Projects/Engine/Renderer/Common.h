
#ifndef Renderer_Common_h
#define Renderer_Common_h


// Library includes
#include <cassert>

// Project includes

// Forward declarations

// Namespace declarations


namespace Core {


class Facing
{
public:
	enum E {
		Both = 0,
		Back = 1,
		Front = 2,
	};

	static E convert(int f) {
		switch ( f ) {
			case 0: return Both;
			case 1: return Back;
			case 2: return Front;
		}

		assert(!"invalid <Facing::E>!");
		return Both;
	}
};


}


#endif
