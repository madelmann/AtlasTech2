
#ifndef _Interfaces_Core_Id_h_
#define _Interfaces_Core_Id_h_


// Library includes
#include <cassert>

// Project includes
#include "Types.h"

// Forward declarations

// Namespace declarations


namespace Core {


class Id
{
public:
	AEid id() const {
		return mId;
	}
	void setId(AEid id) {
		// only allow setting of id if non has been set yet
		assert( mId == invalidID );
		mId = id;
	}

public:
	bool operator == (const Id& other) { return (this->id() == other.id()); }
	bool operator <  (const Id& other) { return (this->id() < other.id());	}
	bool operator >  (const Id& other) { return (this->id() > other.id());	}

protected:
	Id(AEid id = invalidID)
	: mId(id)
	{ }

private:
	AEid	mId;
};


}


#endif
