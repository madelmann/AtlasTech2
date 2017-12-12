
// Header
#include "AABBoxNoTranslate.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>

// Namespace decalarations


namespace Mesh {
namespace Volume {


AABBoxNoTranslateVolume::AABBoxNoTranslateVolume(const vector3f& size)
: IVolume(Type::AABBoxNoTranslate, size)
{
}

void AABBoxNoTranslateVolume::render(bool solid)
{
	aePushMatrix();
		aePushAttrib(GL_ALL_ATTRIB_BITS);

		//aeTranslate(mTranslation);

		aeColor3f(1.f, 1.f, 1.f);
		aeDisable(GL_CULL_FACE, true);
		aeDisable(GL_LIGHTING, true);
		aeDisable(GL_TEXTURE, true);

		glLineWidth(2.f);

		if ( solid ) {
			aeBegin(GL_QUADS);
				// Bottom
				aeVertex3f(0.f,		0.f,	0.f);
				aeVertex3f(mSize.x,	0.f,	0.f);
				aeVertex3f(mSize.x,	0.f,	mSize.z);
				aeVertex3f(0.f,		0.f,	mSize.z);

				// Top
				aeVertex3f(0.f,		mSize.y,	0.f);
				aeVertex3f(mSize.x,	mSize.y,	0.f);
				aeVertex3f(mSize.x,	mSize.y,	mSize.z);
				aeVertex3f(0.f,		mSize.y,	mSize.z);

				// Front
				aeVertex3f(0.f,		0.f,		0.f);
				aeVertex3f(0.f,		mSize.y,	0.f);
				aeVertex3f(mSize.x,	mSize.y,	0.f);
				aeVertex3f(mSize.x,	0.f,		0.f);

				// Back
				aeVertex3f(0.f,		0.f,		mSize.z);
				aeVertex3f(0.f,		mSize.y,	mSize.z);
				aeVertex3f(mSize.x,	mSize.y,	mSize.z);
				aeVertex3f(mSize.x,	0.f,		mSize.z);

				// Left
				aeVertex3f(0.f,		0.f,		0.f);
				aeVertex3f(0.f,		mSize.y,	0.f);
				aeVertex3f(0.f,		mSize.y,	mSize.z);
				aeVertex3f(0.f,		0.f,		mSize.z);

				// Right
				aeVertex3f(mSize.x,	0.f,		0.f);
				aeVertex3f(mSize.x,	mSize.y,	0.f);
				aeVertex3f(mSize.x,	mSize.y,	mSize.z);
				aeVertex3f(mSize.x,	0.f,		mSize.z);
			aeEnd();
		}
		else {
			aeBegin(GL_LINE_LOOP);
				// Bottom
				aeVertex3f(0.f,		0.f,	0.f);
				aeVertex3f(mSize.x,	0.f,	0.f);
				aeVertex3f(mSize.x,	0.f,	mSize.z);
				aeVertex3f(0.f,		0.f,	mSize.z);
			aeEnd();

			aeBegin(GL_LINE_LOOP);
				// Top
				aeVertex3f(0.f,		mSize.y,	0.f);
				aeVertex3f(mSize.x,	mSize.y,	0.f);
				aeVertex3f(mSize.x,	mSize.y,	mSize.z);
				aeVertex3f(0.f,		mSize.y,	mSize.z);
			aeEnd();

			aeBegin(GL_LINE_LOOP);
				// Front
				aeVertex3f(0.f,		0.f,		0.f);
				aeVertex3f(0.f,		mSize.y,	0.f);
				aeVertex3f(mSize.x,	mSize.y,	0.f);
				aeVertex3f(mSize.x,	0.f,		0.f);
			aeEnd();

			aeBegin(GL_LINE_LOOP);
				// Back
				aeVertex3f(0.f,		0.f,		mSize.z);
				aeVertex3f(0.f,		mSize.y,	mSize.z);
				aeVertex3f(mSize.x,	mSize.y,	mSize.z);
				aeVertex3f(mSize.x,	0.f,		mSize.z);
			aeEnd();

			aeBegin(GL_LINE_LOOP);
				// Left
				aeVertex3f(0.f,		0.f,		0.f);
				aeVertex3f(0.f,		mSize.y,	0.f);
				aeVertex3f(0.f,		mSize.y,	mSize.z);
				aeVertex3f(0.f,		0.f,		mSize.z);
			aeEnd();

			aeBegin(GL_LINE_LOOP);
				// Right
				aeVertex3f(mSize.x,	0.f,		0.f);
				aeVertex3f(mSize.x,	mSize.y,	0.f);
				aeVertex3f(mSize.x,	mSize.y,	mSize.z);
				aeVertex3f(mSize.x,	0.f,		mSize.z);
			aeEnd();
		}

		aePopAttrib();
	aePopMatrix();

}


}
}
