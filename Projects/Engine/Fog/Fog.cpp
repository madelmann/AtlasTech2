
// Header
#include "Fog.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include "Settings.h"

// Namespace declarations


namespace FogSpace {


Fog::Fog(Settings *s)
: IAttribute("fog"),
  mSettings(s)
{
}

void Fog::bind()
{
	// Minimize state changes
	if ( !mSettings->enabled() ) {
		if ( mSettings->hasChanged() ) {
			aeDisable(GL_FOG);
		}

		return;
	}

	if ( mSettings->hasChanged() ) {
		float tmp[4];
		tmp[0] = mSettings->color().x;
		tmp[1] = mSettings->color().y;
		tmp[2] = mSettings->color().z;
		tmp[3] = mSettings->color().w;

		glFogfv(GL_FOG_COLOR,	tmp);
		glFogf (GL_FOG_DENSITY,	mSettings->density());
		glFogf (GL_FOG_END,		mSettings->end());
		glFogi (GL_FOG_MODE,	mSettings->mode());
		glFogf (GL_FOG_START,	mSettings->start());
	}

	aeEnable(GL_FOG);
}

void Fog::unbind()
{
	aeDisable(GL_FOG);
}


}
