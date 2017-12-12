
#ifndef Terrain_Settings_h
#define Terrain_Settings_h


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Terrain {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

public:
	void clear() {
		mDebugMode = false;
		mEnabled = false;
		mPosition = vector3f();
		mRenderNormals = false;
		mScale = vector3f();
		mSize = vector3f();
		mSubdivisions = vector2f();
	}

	bool debugMode() const {
		return mDebugMode;
	}
	void debugMode(bool state) {
		mDebugMode = state;
	}

	bool enabled() const {
		return mEnabled;
	}
	void enabled(bool state) {
		mEnabled = state;
	}

	const vector3f& position() const {
		return mPosition;
	}
	void position(const vector3f& position) {
		mPosition = position;
	}

	bool renderNormals() const {
		return mRenderNormals;
	}
	void renderNormals(bool state) {
		mRenderNormals = state;
	}

	const vector3f& scale() const {
		return mScale;
	}
	void scale(const vector3f& scale) {
		mScale = scale;
	}

	const vector3f& size() const {
		return mSize;
	}
	void size(const vector3f& size) {
		mSize = size;
	}

	const vector2f& subdivisions() const {
		return mSubdivisions;
	}
	void subdivisions(const vector2f& subdivisions) {
		mSubdivisions = subdivisions;
	}

	bool useSubdividedLayers() const {
		return (mSubdivisions.x > 1 && mSubdivisions.y > 1);
	}

protected:

private:
	bool		mDebugMode;
	bool		mEnabled;
	vector3f	mPosition;
	bool		mRenderNormals;
	vector3f	mScale;
	vector3f	mSize;
	vector2f	mSubdivisions;
};


}


#endif
