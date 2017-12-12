
#ifndef _Camera_EditorCamera_h_
#define _Camera_EditorCamera_h_


// Library includes

// Project includes
#include "BasicCamera.h"

// Forward declarations

// Namespace declarations


class EditorCamera : public BasicCamera
{
public:
	EditorCamera();
	virtual ~EditorCamera() { }

	// Viewer implementation
	//{
	void update(float elapsedTime);						// This updates the camera's view and other data (Should be called each frame)
	//{

	void checkForMovement();							// This checks for keyboard movement
	void move(float speed);								// This will move the camera forward or backward depending on the speed
	void setViewByMouse();								// This moves the camera's view by the mouse movements (First person view)
	void strafe(float speed);							// This strafes the camera left or right depending on the speed (+/-)

	bool isFollowMouseView() const;
	void setFollowMouseView(bool flag);

protected:

private:
	bool	mFollowMouseView;
};


#endif
