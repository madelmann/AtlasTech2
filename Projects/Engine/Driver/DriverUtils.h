
#ifndef _Driver_DriverUtils_h_
#define _Driver_DriverUtils_h_


// Library includes
#include <string>

// Projects includes
#include <Math/Vector2.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


// Public declarations
vector3f glPosition3d(const vector2l& pos);
void glRenderText3v(const vector3f& pos, const std::string& text);


#endif
