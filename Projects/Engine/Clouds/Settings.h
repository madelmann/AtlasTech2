
#ifndef _Clouds_Settings_h_
#define _Clouds_Settings_h_


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/ISettingsService.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace CloudSpace {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

	void clear() {
		mColor = vector3f(1.f, 1.f, 1.f);
		mColorMap = "";
		mCount = 0;
		mEnabled = false;
		mEnd = vector3f();
		mHeight = 0.f;
		mMaterial = "";
		mScale = vector3f(1.f, 1.f, 1.f);
		mSizeMax = vector3f();
		mSizeMin = vector3f();
		mStart = vector3f();
	}

	const vector3f& color() const {
		return mColor;
	}
	void color(const vector3f& color) {
		changed();
		mColor = color;
	}

	const std::string& colorMap() const {
		return mColorMap;
	}
	void colorMap(const std::string& colormap) {
		changed();
		mColorMap = colormap;
	}

	int count() const {
		return mCount;
	}
	void count(int count) {
		changed();
		mCount = count;
	}

	bool enabled() const {
		return mEnabled;
	}
	void enabled(bool flag) {
		changed();
		mEnabled = flag;
	}

	const vector3f& end() const {
		return mEnd;
	}
	void end(const vector3f& end) {
		changed();
		mEnd = end;
	}

	float height() const {
		return mHeight;
	}
	void height(float height) {
		changed();
		mHeight = height;
	}

	const std::string& material() const {
		return mMaterial;
	}
	void material(const std::string& material) {
		changed();
		mMaterial = material;
	}

	const vector3f& scale() const {
		return mScale;
	}
	void scale(const vector3f& scale) {
		changed();
		mScale = scale;
	}

	const vector3f& sizeMax() const {
		return mSizeMax;
	}
	void sizeMax(const vector3f& size) {
		changed();
		mSizeMax = size;
	}

	const vector3f& sizeMin() const {
		return mSizeMin;
	}
	void sizeMin(const vector3f& min) {
		mSizeMin = min;
	}

	const vector3f& start() const {
		return mStart;
	}
	void start(const vector3f& start) {
		changed();
		mStart = start;
	}

private:
	vector3f	mColor;
	std::string	mColorMap;
	int			mCount;
	bool		mEnabled;
	vector3f	mEnd;
	float		mHeight;
	std::string	mMaterial;
	vector3f	mScale;
	vector3f	mSizeMax;
	vector3f	mSizeMin;
	vector3f	mStart;
};


}


#endif
