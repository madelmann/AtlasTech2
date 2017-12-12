
// Header
#include "Editor.h"

// Project includes
#include <Scene/SceneGraphObject.h>


Editor::Editor()
: mEnabled(false),
  mMesh(0),
  mOffset(64.f, 64.f),
  mPosition(0, 0),
  mRasterSize(128.f, 128.f)
{
}

void Editor::buildUnit()
{
	// build a "tower" at our current position
}

void Editor::enabled(bool state)
{
	mEnabled = state;

	if ( mMesh ) {
		if ( mEnabled ) {
			mMesh->show();
		}
		else {
			mMesh->hide();
		}
	}
}

void Editor::init()
{
	// load all meshes
}

void Editor::position(const Position& position)
{
	mPosition = position;
	
	if ( mMesh ) {
		mMesh->setPosition(vector3f(
			mPosition.x * mRasterSize.x + mOffset.x,
			10.f,
			mPosition.y * mRasterSize.y + mOffset.y
		));
	}
}
