
#ifndef _Camera_BasicCamera_h_
#define _Camera_BasicCamera_h_


// Library includes

// Project includes
#include <Camera\Viewer.h>

// Forward declarations

// Namespacde declarations


class BasicCamera : public CameraSpace::Viewer
{
public:
	BasicCamera() { }
	virtual ~BasicCamera() { }

	// These are are data access functions for our camera's data
	void PositionCamera(float positionX, float positionY, float positionZ,		// This changes the position, view, and up vector of the camera.
			 		    float viewX,     float viewY,     float viewZ,			// This is primarily used for initialization
						float upVectorX, float upVectorY, float upVectorZ);
	void RotateAroundPoint(vector3f vCenter, float X, float Y, float Z);		// This rotates the camera around a point (I.E. your character)
	void RotateView(float angle, float X, float Y, float Z);					// This rotates the camera's view around the position depending on the values passed in.

protected:
	vector3f	mCameraAngle;

private:

};


#endif
