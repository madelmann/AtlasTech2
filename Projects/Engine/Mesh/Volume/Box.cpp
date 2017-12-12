
// Header
#include "Box.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace decalarations


namespace Mesh {
namespace Volume {


BoxVolume::BoxVolume(const vector3f& size)
: IVolume(Type::Box, size)
{
}

void BoxVolume::render(bool solid)
{
	aePushMatrix();
		aePushAttrib(GL_ALL_ATTRIB_BITS);

		aeTranslate(mTranslation);

		aeColor4f(1.f, 1.f, 1.f, 1.f);
		aeDisable(GL_CULL_FACE);
		aeDisable(GL_LIGHTING);
		aeDisable(GL_TEXTURE);

		glLineWidth(2.f);

		if ( !solid ) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);								// Draw frontfacing polygons as wireframe
		}

		aeBegin(GL_QUADS);
			// Bottom
			aeVertex3f(-mCenter.x, -mCenter.y,  mCenter.z);
			aeVertex3f( mCenter.x, -mCenter.y,  mCenter.z);
			aeVertex3f( mCenter.x, -mCenter.y, -mCenter.z);
			aeVertex3f(-mCenter.x, -mCenter.y, -mCenter.z);

			// Top
			aeVertex3f( mCenter.x,  mCenter.y, -mCenter.z);
			aeVertex3f( mCenter.x,  mCenter.y,  mCenter.z);
			aeVertex3f(-mCenter.x,  mCenter.y,  mCenter.z);
			aeVertex3f(-mCenter.x,  mCenter.y, -mCenter.z);

			// Front
			aeVertex3f(-mCenter.x, -mCenter.y, -mCenter.z);
			aeVertex3f(-mCenter.x,  mCenter.y, -mCenter.z);
			aeVertex3f(-mCenter.x,  mCenter.y,  mCenter.z);
			aeVertex3f(-mCenter.x, -mCenter.y,  mCenter.z);

			// Left
			aeVertex3f( mCenter.x, -mCenter.y, -mCenter.z);
			aeVertex3f( mCenter.x,  mCenter.y, -mCenter.z);
			aeVertex3f(-mCenter.x,  mCenter.y, -mCenter.z);
			aeVertex3f(-mCenter.x, -mCenter.y, -mCenter.z);

			// Back
			aeVertex3f( mCenter.x, -mCenter.y,  mCenter.z);
			aeVertex3f( mCenter.x,  mCenter.y,  mCenter.z);
			aeVertex3f( mCenter.x,  mCenter.y, -mCenter.z);
			aeVertex3f( mCenter.x, -mCenter.y, -mCenter.z);

			// Right
			aeVertex3f( mCenter.x,  mCenter.y,  mCenter.z);
			aeVertex3f( mCenter.x, -mCenter.y,  mCenter.z);
			aeVertex3f(-mCenter.x, -mCenter.y,  mCenter.z);
			aeVertex3f(-mCenter.x,  mCenter.y,  mCenter.z);
		aeEnd();

		aePopAttrib();
	aePopMatrix();
}


}
}
