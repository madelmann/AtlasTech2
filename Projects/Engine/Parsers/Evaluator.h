
#ifndef _Parsers_Evaluator_h_
#define _Parsers_Evaluator_h_


// Library includes

// Project includes
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

// Forward declarations
class Valueizer;

// Namespace declarations


class Evaluator
{
public:
	static vector2f toVector2f(Valueizer *t);
	static vector3f toVector3f(Valueizer *t);
	static vector4f toVector4f(Valueizer *t);

protected:

private:

};


#endif
