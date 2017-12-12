
#ifndef _Renderer_FrameSettings_h_
#define _Renderer_FrameSettings_h_


// Library includes

// Project includes
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Renderer {


class FrameSettings
{
public:
	FrameSettings();
	virtual ~FrameSettings() { }

public:
	const vector3f& color() const;
	void color(const vector3f& color);

	float viewWidth() const;
	void viewWidth(float viewWidth);

protected:

private:
	vector3f	mColor;
	float		mViewWidth;
};


}


#endif
