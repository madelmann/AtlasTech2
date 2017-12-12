
#ifndef _Interfaces_Appearence_IAttribute_h_
#define _Interfaces_Appearence_IAttribute_h_


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


class IAttribute
{
public:
	virtual ~IAttribute() { }

public:
	virtual void bind() = 0;
	virtual void unbind() = 0;

public:
	bool enabled() const { return mEnabled; }
	void enabled(bool state) { mEnabled = state; }

	const std::string& name() const { return mName; }

protected:
	IAttribute(const std::string& name)
	: mEnabled(false),
	  mName(name)
	{ }

private:
	bool		mEnabled;
	std::string	mName;
};


#endif
