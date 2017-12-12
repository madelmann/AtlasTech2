
#ifndef _Sound_Sound2D_h_
#define _Sound_Sound2D_h_


// Library includes

// Project includes
#include "Sound.h"
#include <Math/Vector2.h>

// Forward declarations

// Namespace declarations


namespace SoundSpace {


class Sound2D : public Sound
{
public:
	Sound2D();
	virtual ~Sound2D();

	const vector2f position() const;
	void position(const vector2f& position);

protected:
	float	mPositionX;
	float	mPositionY;

private:

};


}


#endif
