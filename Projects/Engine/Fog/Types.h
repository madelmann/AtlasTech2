
#ifndef Fog_Types_h
#define Fog_Types_h


// Library includes
#include <cassert>

// Project includes

// Forward declarations

// Namespace declarations


namespace FogSpace {


class FogMode
{
public:
	enum E {
		Linear = 0x2601,
		Exponential = 0x0800,
		Exponential2 = 0x0801
	};

	static E convert(int mode) {
		switch ( mode ) {
			case 0: return Linear;
			case 1: return Exponential;
			case 2: return Exponential2;
		}

		assert(!"invalid mode provided!");
		return Linear;
	}

};


}


#endif
