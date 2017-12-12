
#ifndef _Interfaces_Renderer_IScreenWriter_h_
#define _Interfaces_Renderer_IScreenWriter_h_


// Library includes
#include <string>

// Project includes
#include <Driver/GLWrapper.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Core {


class IScreenWriter
{
public:
	virtual ~IScreenWriter() { }

	virtual void write(float x, float y, const std::string& text, aeint time = 0, const vector3f& color = vector3f(1.f, 1.f, 1.f)) = 0;
};


}


#endif
