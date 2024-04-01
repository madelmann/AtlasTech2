
#ifndef _Camera_Viewer_h_
#define _Camera_Viewer_h_


// Library includes

// Project includes
#include <Interfaces/Camera/IViewer.h>

// Forward declarations

// Namespace declarations


namespace CameraSpace {


class Viewer : public IViewer
{
public:
	Viewer();
	virtual ~Viewer();

	virtual void look();
	virtual void update(float elapsedTime);

	virtual const float& fov() const;
	virtual void fov(const float& fov);

	virtual vector3f position() const;
	virtual void position(const vector3f& pos);

	virtual vector3f rotation() const;
	virtual void rotation(const vector3f& rotation);

	virtual const vector3f& view() const;
	virtual void view(const vector3f& view);

	virtual const vector3f& upVector() const;
	virtual void upVector(const vector3f& up);

	virtual void convertRotationToView();
	virtual void convertViewToRotation();

protected:
	float		mFOV;
	vector3f	mPosition;
	vector3f	mRotation;
	vector3f	mStrafe;
	vector3f	mView;
	vector3f	mUpVector;

private:

};


}


#endif
