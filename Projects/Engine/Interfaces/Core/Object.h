
#ifndef _Interfaces_Core_Object_h_
#define _Interfaces_Core_Object_h_


// Library includes

// Project includes
#include "Id.h"
#include "Name.h"

// Forward declarations

// Namespace declarations


namespace Core {


class Object : public Id,
			   public Name
{
public:
	virtual ~Object() {
		unload();
	}

public:
	virtual void init() { }
	virtual void manage() { }
	virtual void update(float /*elapsedTime*/) { }

protected:
	Object(AEid id = Core::invalidID, const std::string& name = "")
	: Id(id),
	  Name(name)
	{ }

protected:
	virtual void unload() { }

private:

};

}


#endif
