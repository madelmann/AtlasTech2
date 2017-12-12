
#ifndef _Interfaces_Core_Name_h_
#define _Interfaces_Core_Name_h_


// Library includes
#include <cassert>
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace Core {


class Name
{
public:
	Name(const std::string& name)
	: mName(name)
	{ }
	virtual ~Name() { }

public:
	const std::string& name() const {
		return mName;
	}
	void name(const std::string& name) {
		// only allow setting the name if none has been set yet
		assert(mName.empty());
		mName = name;
	}

protected:
	void reassign(const std::string& name) {
		mName = name;
	}

private:
	std::string mName;
};


}


#endif
