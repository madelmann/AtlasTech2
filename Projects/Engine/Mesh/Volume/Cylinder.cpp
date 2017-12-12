
// Header
#include "Cylinder.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace Mesh {
namespace Volume {


CylinderVolume::CylinderVolume(const vector3f& size)
: IVolume(Type::Cylinder, size)
{
}

void CylinderVolume::render(bool solid)
{
	aePushMatrix();
		aePushAttrib(GL_ALL_ATTRIB_BITS);

		aeTranslate(mTranslation + mCenter);

		aeColor3f(1.f, 1.f, 1.f);
		aeDisable(GL_CULL_FACE);
		aeDisable(GL_LIGHTING);
		aeDisable(GL_TEXTURE);

		glLineWidth(2.f);

		static GLUquadricObj *myCylinder = gluNewQuadric();
		gluQuadricTexture(myCylinder, GL_FALSE);

		if ( !solid ) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);								// Draw frontfacing polygons as wireframe
		}

		vector3f size = this->size();
		gluCylinder(myCylinder, size.x, size.y, size.z, 12, 6);

		aePopAttrib();
	aePopMatrix();
}

vector3f CylinderVolume::size()
{
	float f;
	f = mSize.x;
	if ( mSize.z > f ) {
		f = mSize.z;
	}

	f /= 2;

	return vector3f(f, mSize.y, f);
}


}
}
