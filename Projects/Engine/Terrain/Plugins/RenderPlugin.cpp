
// Header
#include "RenderPlugin.h"

// Library includes
#include <cassert>

// Project includes
#include <Driver/GLWrapper.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Misc/Colors.h>
#include <Terrain/Settings.h>

// Namespace declarations


namespace Terrain {


RenderPlugin::RenderPlugin()
: IRendererPlugin(Core::invalidID, "Terrain::RenderPlugin"),
  mSettings(0)
{
}

void RenderPlugin::connectSettings(Settings *s)
{
	assert(s);
	assert(!mSettings);

	mSettings = s;
}

void RenderPlugin::render()
{
	assert(mSettings);		// just to be sure...

	vector3f size = mSettings->size();
	vector2f sub = mSettings->subdivisions();

	aeColor(Color::white);

	//aeForceDisable(GL_LIGHTING);
	//aeForceDisable(GL_TEXTURE);
	glLineWidth(3.f);

	aeBegin(GL_LINES);
		for ( int x = 0; x <= sub.x; ++x ) {
			aeVertex3f(x * (size.x / sub.x),	0.f,	0.f		);
			aeVertex3f(x * (size.x / sub.x),	0.f,	size.z	);
		}
		for ( int z = 0; z <= sub.y; ++z ) {
			aeVertex3f(0.f,		0.f,	z * (size.z / sub.y)	);
			aeVertex3f(size.x,	0.f,	z * (size.z / sub.y)	);
		}
	aeEnd();
}


}
