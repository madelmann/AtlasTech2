
// Header
#include "Utils.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace Mesh {


void applyFaceCulling(Core::Facing::E mode)
{
	switch ( mode ) {
		case Core::Facing::Both:
			aeForceDisable(GL_CULL_FACE);
			break;
		case Core::Facing::Back:
			aeForceEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		case Core::Facing::Front:
			aeForceEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
	}
}


}
