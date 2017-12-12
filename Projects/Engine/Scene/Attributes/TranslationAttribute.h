
#ifndef _Scene_Attributes_TranslationAttribute_h_
#define _Scene_Attributes_TranslationAttribute_h_


// Library includes

// Project includes
#include <Interfaces/Scene/IAttribute.h>
#include <Math/Vector3.h>

// Forward declarations

// Namespace declarations


namespace Scene {


class TranslationAttribute : public IAttribute
{
public:
	TranslationAttribute(const vector3f& value);

public:
	void apply();

protected:

private:
	vector3f	mValue;
};


}


#endif
