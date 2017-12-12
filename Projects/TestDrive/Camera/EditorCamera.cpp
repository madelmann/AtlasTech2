
// Header
#include "EditorCamera.h"

// Library includes

// Project includes
#include "globals.h"
#include <Core\Engine.h>
#include <Math\Normals.h>
#include <Math\Tools.h>

// Namespace declarations


// This is how fast our camera moves
const float kSpeed = 128.0f;


EditorCamera::EditorCamera()
: BasicCamera(),
  mFollowMouseView(false)
{
}

//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the input faster than in the WinProc()
/////
//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void EditorCamera::checkForMovement()
{
	// Once we have the frame interval, we find the current speed
	float speed = kSpeed * mEngine->getFrameTime();

	// Check if we hit the "run" key
	if ( GetKeyState(VK_SHIFT) & 0x80 ) {
		speed *= 4;
	}

	// Check if we hit the "backward" key
	if ( GetKeyState('S') & 0x80 ) {
		move(-speed);
	}

	// Check if we hit the "forward" key
	if ( GetKeyState('W') & 0x80 ) {
		move(speed);
	}

	// Check if we hit the "strafe left" key
	if ( GetKeyState('A') & 0x80 ) {
		strafe(-speed);
	}

	// Check if we hit the "strafe right" key
	if ( GetKeyState('D') & 0x80 ) {
		strafe(speed);
	}}

bool EditorCamera::isFollowMouseView() const
{
	return mFollowMouseView;
}

///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This will move the camera forward or backward depending on the speed
/////
///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void EditorCamera::move(float speed)
{
	// Get the current view vector (the direction we are looking)
	vector3f vVector = mView - mPosition;
	vVector = normalize(vVector);

	mPosition.x += vVector.x * speed;		// Add our acceleration to our position's X
	mPosition.y += vVector.y * speed;		// Add our acceleration to our position's Y
	mPosition.z += vVector.z * speed;		// Add our acceleration to our position's Z
	mView.x += vVector.x * speed;			// Add our acceleration to our view's X
	mView.y += vVector.y * speed;			// Add our acceleration to our view's Y
	mView.z += vVector.z * speed;			// Add our acceleration to our view's Z
}

void EditorCamera::setFollowMouseView(bool flag)
{
	mFollowMouseView = flag;
}

///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This allows us to look around using the mouse, like in most first person games.
/////
///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void EditorCamera::setViewByMouse()
{
	POINT mousePos;									// This is a window structure that holds an X and Y
	int middleX = mEngine->getScreen()->getCenterX();			// This is a binary shift to get half the width
	int middleY = mEngine->getScreen()->getCenterY();			// This is a binary shift to get half the height
	float angleY = 0.0f;							// This is the direction for looking up or down
	float angleZ = 0.0f;							// This will be the value we need to rotate around the Y axis (Left and Right)
	static float currentRotX = 0.0f;
	
	// Get the mouse's current X,Y position
	GetCursorPos(&mousePos);						
	
	// If our cursor is still in the middle, we never moved... so don't update the screen
	if( (mousePos.x == middleX) && (mousePos.y == middleY) )
		return;

	// Set the mouse position to the middle of our window
	SetCursorPos(middleX, middleY);
	
	// Get the direction the mouse moved in, but bring the number down to a reasonable amount
	angleY = (float)( (middleX - mousePos.x) ) / g_fMouseSpeedDown;
	angleZ = (float)( (middleY - mousePos.y) ) / g_fMouseSpeedDown;

	mCameraAngle.y += angleY * 180 / fPi;
	if ( mCameraAngle.y >= 360.0f ) {
		mCameraAngle.y -= 360.0f;
	}
	if ( mCameraAngle.y < 0 ) {
		mCameraAngle.y += 360.0f;
	}

	mCameraAngle.z += angleZ * 180 / fPi;
	if ( mCameraAngle.z >= 360.0f ) {
		mCameraAngle.z -= 360.0f;
	}
	if ( mCameraAngle.z < 0 ) {
		mCameraAngle.z += 360.0f;
	}

	static float lastRotX = 0.0f; 
 	lastRotX = currentRotX; // We store off the currentRotX and will use it in when the angle is capped
	
	// Here we keep track of the current rotation (for up and down) so that
	// we can restrict the camera from doing a full 360 loop.
	currentRotX += angleZ;
 
	//if ( mEngineStates->isPlayMode() ) {
		// If the current rotation (in radians) is greater than 1.5, we want to cap it.
		if(currentRotX > 1.5f)     
		{
			currentRotX = 1.5f;

			// Rotate by remaining angle if there is any
			if(lastRotX != 1.5f) 
			{
				// To find the axis we need to rotate around for up and down
				// movements, we need to get a perpendicular vector from the
				// camera's view vector and up vector.  This will be the axis.
				// Before using the axis, it's a good idea to normalize it first.
				vector3f vAxis = cross(mView - mPosition, mUpVector);
				vAxis = normalize(vAxis);

				// rotate the camera by the remaining angle (1.0f - lastRotX)
				RotateView( 1.5f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
			}
		}
		// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
		else if(currentRotX < -1.0f)
		{
			currentRotX = -1.0f;

			// Rotate by the remaining angle if there is any
			if(lastRotX != -1.0f)
			{
				// To find the axis we need to rotate around for up and down
				// movements, we need to get a perpendicular vector from the
				// camera's view vector and up vector.  This will be the axis.
				// Before using the axis, it's a good idea to normalize it first.
				vector3f vAxis = cross(mView - mPosition, mUpVector);
				vAxis = normalize(vAxis);

				// rotate the camera by ( -1.0f - lastRotX)
				RotateView( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
			}
		}
		// Otherwise, we can rotate the view around our position
		else 
		{	
			// To find the axis we need to rotate around for up and down
			// movements, we need to get a perpendicular vector from the
			// camera's view vector and up vector.  This will be the axis.
			// Before using the axis, it's a good idea to normalize it first.
			vector3f vAxis = cross(mView - mPosition, mUpVector);
			vAxis = normalize(vAxis);

			// Rotate around our perpendicular axis
			RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
		}
	//}
	//else
	//{
	//	// To find the axis we need to rotate around for up and down
	//	// movements, we need to get a perpendicular vector from the
	//	// camera's view vector and up vector.  This will be the axis.
	//	// Before using the axis, it's a good idea to normalize it first.
	//	vector3f vAxis = cross(mView - mPosition, mUpVector);
	//	vAxis = Normalize(vAxis);

	//	// Rotate around our perpendicular axis
	//	RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	//}

	// Always rotate the camera around the y-axis
	RotateView(angleY, 0, 1, 0);
}

///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This strafes the camera left and right depending on the speed (-/+)
/////
///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void EditorCamera::strafe(float speed)
{
	mPosition.x += m_vStrafe.x * speed;		// Add our acceleration to our position's X
	mPosition.y += m_vStrafe.y * speed;		// Add our acceleration to our position's Y
	mPosition.z += m_vStrafe.z * speed;		// Add our acceleration to our position's Z
	mView.x += m_vStrafe.x * speed;			// Add our acceleration to our view's X
	mView.y += m_vStrafe.y * speed;			// Add our acceleration to our view's Y
	mView.z += m_vStrafe.z * speed;			// Add our acceleration to our view's Z
}

void EditorCamera::update(float /*elapsedTime*/) 
{
	// Initialize a variable for the cross product result
	vector3f vCross = cross(mView - mPosition, mUpVector);

	// Normalize the strafe vector
	m_vStrafe = normalize(vCross);

	if( mFollowMouseView ) {
		if ( !mEngine->isConsoleVisible() ) {
			setViewByMouse();													// Move the camera's view by the mouse

			checkForMovement();													// This checks to see if the keyboard was pressed
		}
	}
}
