
#ifndef _Scene_Attributes_ColorAttribute_h_
#define _Scene_Attributes_ColorAttribute_h_


// Library includes

// Project includes
#include <Interfaces/Scene/IAttribute.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Scene {


class ColorAttribute : public IAttribute
{
public:
	ColorAttribute(const vector3f& value);

public:
	void apply();

protected:

private:
	vector3f	mValue;
};


}


#endif
