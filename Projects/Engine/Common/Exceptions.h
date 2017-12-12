
#ifndef _Common_Exceptions_h_
#define _Common_Exceptions_h_


// Library includes
#include <exception>
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {
namespace Exceptions {


class Exception : public std::exception
{
public:
	Exception(const std::string& text)
	: std::exception(),
	  mText(text)
	{ }
	virtual ~Exception() { }

	const std::string& what() {
		return mText;
	}

protected:

private:
	std::string	mText;
};


}
}


#endif
