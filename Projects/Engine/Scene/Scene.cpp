
// Header
#include "Scene.h"

// Library includes

// Project includes
#include "Settings.h"
#include <Frustum/Frustum.h>
#include <Interfaces/Camera/IViewer.h>
#include <Math/Tools.h>
#include <Renderer/RenderObject.h>

// Namespace declarations


namespace Scene {


Scene::Scene(const Common::Logger *p)
: Common::Logger(p, "Scene"),
  mFrustum(0),
  mSettings(0),
  mViewer(0)
{
	debug("Starting...");
}

Scene::~Scene()
{
	shutdown();
}

void Scene::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mSettings);
	assert(mViewer);

	mConfigurationComplete = true;
}

void Scene::connectFrustum(Frustum *f)
{
	mFrustum = f;
}

void Scene::connectSettings(Settings *s)
{
	mSettings = s;
}

void Scene::connectViewer(CameraSpace::IViewer *v)
{
	mViewer = v;
}

float Scene::getDistanceToViewer(const vector3f& pos)
{
	vector3f vpos = mViewer->position();
	vector3f dist = vpos - pos;

	return fastsqrtf(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
}

float Scene::getFadeValue(SceneGraphObject *sgo)
{
	if ( !sgo ) {
		return false;
	}

	float begin = mSettings->distanceFade();
	float end = mSettings->distanceHide();
	float pos = getDistanceToViewer(sgo->getPosition()) - sgo->getRadius();

	if ( begin == end ) {
		end += 1.f;
	}

	return (pos - begin) / (end - begin);
}

bool Scene::isInViewingDistance(SceneGraphObject *sgo)
{
	if ( !sgo ) {
		return false;
	}

	float dist = getDistanceToViewer(sgo->getPosition()) - sgo->getRadius();

	return ( mSettings->distanceHide() > dist );
}

void Scene::render()
{
	for ( NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it ) {
		if ( !(*it)->alwaysVisible() && !mFrustum->volumeInFrustum((*it)->getBoundingVolume()) ) {
			// volume not in frustum
			continue;
		}

		float fadeValue = getFadeValue((*it));
		if ( fadeValue > 1.f ) {
			// distance is higher than max visible distance
			continue;
		}

		if ( fadeValue > 0.f && fadeValue < 1.f ) {
			// begin the fading thing...
			aeEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			vector4f color;
			color.x = 1.f - fadeValue;
			color.y = 1.f - fadeValue;
			color.z = 1.f - fadeValue;
			color.w = 1.f - fadeValue;

			aeColor(color);
		}

		if ( mEditorMode && (*mEditorMode) ) {
			(*it)->renderBoundingVolume(false);
		}

		(*it)->render();

		if ( fadeValue > 0.f ) {
			// stop the fading thing...
			aeDisable(GL_BLEND);
		}
	}
}

void Scene::shutdown()
{
	debug("Stopping...");

	clear();
}


}
