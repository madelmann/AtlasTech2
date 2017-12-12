
#ifndef _Camera_BasicCamera_h_
#define _Camera_BasicCamera_h_


// Library includes

// Project includes
#include <Camera\Viewer.h>
#include <Interfaces\Common\AComponent.h>

// Forward declarations
namespace Core {
	class Engine;
}

// Namespacde declarations


class BasicCamera : public Common::AComponent,
					public CameraSpace::Viewer
{
public:
	BasicCamera();
	virtual ~BasicCamera() { }

	// IComponent implementation
	//{
	void configurationComplete();
	void shutdown();
	//}

	// Connectors
	//{
	void connectEngine(Core::Engine *e);
	//}

	// These are are data access functions for our camera's data
	virtual void look();														// This uses gluLookAt() to tell OpenGL where to look
	void PositionCamera(float positionX, float positionY, float positionZ,		// This changes the position, view, and up vector of the camera.
			 		    float viewX,     float viewY,     float viewZ,			// This is primarily used for initialization
						float upVectorX, float upVectorY, float upVectorZ);
	void RotateAroundPoint(vector3f vCenter, float X, float Y, float Z);		// This rotates the camera around a point (I.E. your character)
	void RotateView(float angle, float X, float Y, float Z);					// This rotates the camera's view around the position depending on the values passed in.
	void SetSequencePosition(vector3f NewPos);
	void SetSequenceView(vector3f NewView);
	

	vector3f	mGamePosition;
	vector3f	mGameView;
	vector3f	mSequencePosition;
	vector3f	mSequenceView;

protected:
	Core::Engine	*mEngine;

	vector3f	mCameraAngle;

private:

};


#endif
