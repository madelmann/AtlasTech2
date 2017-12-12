
#ifndef _Camera_External_h_
#define _Camera_External_h_


// Library includes

// Project includes
#include <Camera/Viewer.h>
#include <Math/vector3.h>

// Forward declarations

// Namespace declarations


class External : public CameraSpace::Viewer
{
public:
	void look();

	void offset(const vector3f& offset);
	void rotation(const vector3f& rotation); 
	// Setting a rotation overrides a previously setted view

protected:

private:
	vector3f	mOffset;
};


#endif
