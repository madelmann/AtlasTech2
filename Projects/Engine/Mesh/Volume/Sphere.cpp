
// Header
#include "Sphere.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace declarations


namespace Mesh {
namespace Volume {


SphereVolume::SphereVolume(const vector3f& size)
: IVolume(Type::Sphere, size)
{
}

void SphereVolume::render(bool solid)
{
	aePushMatrix();
		aePushAttrib(GL_ALL_ATTRIB_BITS);

		aeTranslate(mTranslation + mCenter);

		aeColor3f(1.f, 1.f, 1.f);
		aeDisable(GL_CULL_FACE);
		aeDisable(GL_LIGHTING);
		aeDisable(GL_TEXTURE);

		glLineWidth(2.f);

		static GLUquadricObj *mySphere = gluNewQuadric();
		gluQuadricTexture(mySphere, GL_FALSE);

		if ( !solid ) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);								// Draw frontfacing polygons as wireframe
		}

		vector3f size = this->size();
		gluSphere(mySphere, radius(), 12, 6);

		aePopAttrib();
	aePopMatrix();
}

vector3f SphereVolume::size()
{
	float f;
	f = mSize.x;
	if ( f < mSize.y ) {
		f = mSize.y;
	}
	if ( f < mSize.z ) {
		f = mSize.z;
	}

	f /= 2;

	return vector3f(f, f, f);
}


}
}
