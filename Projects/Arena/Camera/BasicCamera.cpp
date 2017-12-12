
// Header
#include "BasicCamera.h"

// Library includes

// Project includes
#include <Math/Normals.h>
#include <Math/Tools.h>

// Namespace declarations


BasicCamera::BasicCamera()
{
}

BasicCamera::~BasicCamera()
{
}

void BasicCamera::PositionCamera(float positionX, float positionY, float positionZ,
				  				 float viewX,     float viewY,     float viewZ,
								 float upVectorX, float upVectorY, float upVectorZ)
{
	vector3f vPosition	= vector3f(positionX, positionY, positionZ);
	vector3f vView		= vector3f(viewX, viewY, viewZ);
	vector3f vUpVector	= vector3f(upVectorX, upVectorY, upVectorZ);

	// The code above just makes it cleaner to set the variables.
	// Otherwise we would have to set each variable x y and z.

	mPosition = vPosition;					// Assign the position
	mView     = vView;						// Assign the view
	mUpVector = vUpVector;					// Assign the up vector
}

void BasicCamera::RotateView(float angle, float x, float y, float z)
{
	vector3f vNewView;

	// Get the view vector (The direction we are facing)
	vector3f vView = mView - mPosition;		

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	mView = mPosition + vNewView;
}
