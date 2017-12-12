
#ifndef Interfaces_Scene_IAttribute_h
#define Interfaces_Scene_IAttribute_h


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Scene {


class IAttribute
{
public:
	virtual ~IAttribute() { }

public:
	virtual void apply() = 0;
};



}


#endif
