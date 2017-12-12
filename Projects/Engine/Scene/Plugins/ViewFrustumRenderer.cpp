
// Header
#include "ViewFrustumRenderer.h"

// Library includes
#include <cassert>

// Project includes
#include <Driver/GLWrapper.h>
#include <Frustum/Frustum.h>

// Namespace declarations


namespace Scene {


ViewFrustumRenderer::ViewFrustumRenderer()
: IRendererPlugin(Core::invalidID, "ViewFrustumRenderer"),
  mFrustum(0)
{
}

void ViewFrustumRenderer::connectFrustum(Frustum *f)
{
	assert(f);
	assert(!mFrustum);

	mFrustum = f;
}

void ViewFrustumRenderer::render()
{
return;
	assert(mFrustum);			// just to make sure...

	aeColor3f(1.f, 0.f, 1.f);
	aeForceDisable(GL_LIGHTING);
	aeForceDisable(GL_TEXTURE);
	glLineWidth(3.f);

	glBegin(GL_QUADS);
		
	glEnd();
}


}
