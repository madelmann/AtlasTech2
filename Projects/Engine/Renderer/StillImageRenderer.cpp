
// Header
#include "StillImageRenderer.h"

// Library includes
#include <assert.h>

// Project includes
#include <Core/EngineSettings.h>
#include <Driver/GLWrapper.h>
#include <Texture/Texture.h>


namespace Core {


StillImageRenderer::StillImageRenderer()
: mEngineSettings(0)
{
}

StillImageRenderer::~StillImageRenderer()
{
	shutdown();
}

void StillImageRenderer::configurationComplete()
{
	assert(!mConfigurationComplete);

	assert(mEngineSettings);

	mConfigurationComplete = true;
}

void StillImageRenderer::connectEngineSettings(EngineSettings *s)
{
	assert(s);
	assert(!mEngineSettings);

	mEngineSettings = s;
}

void StillImageRenderer::renderImage(TextureSpace::Texture *texture, float left, float top, float width, float height)
{
	glMatrixMode(GL_PROJECTION);
	aePushMatrix();								// Push on a new matrix so that we can just pop it off to go back to perspective mode
		aeLoadIdentity();						// Reset the current matrix to our identify matrix

		// Pass in our 2D ortho screen coordinates like so (left, right, bottom, top).
		// The last 2 parameters are the near and far planes.
		glOrtho(0, mEngineSettings->getScreen()->getWidth(), 0, mEngineSettings->getScreen()->getHeight(), 0, 10);

		assert(texture);
		texture->bind();

		aeBegin(GL_QUADS);
			aeTexCoord2f(0.0f, 0.0f); aeVertex2f(left, top);
			aeTexCoord2f(1.0f, 0.0f); aeVertex2f(left + width, top);
			aeTexCoord2f(1.0f, 1.0f); aeVertex2f(left + width, top + height);
			aeTexCoord2f(0.0f, 1.0f); aeVertex2f(left, top + height);
		aeEnd();

	aePopMatrix();           
	glMatrixMode(GL_MODELVIEW);					// Go back to our model view matrix like normal
}

void StillImageRenderer::renderImageAtFullScreen(TextureSpace::Texture *texture)
{
	renderImage(texture, 0.0f, 0.0f, (float)mEngineSettings->getScreen()->getWidth(), (float)mEngineSettings->getScreen()->getHeight());
}

void StillImageRenderer::shutdown()
{
}


}
