
#ifndef _JSON_Exceptions_h_
#define _JSON_Exceptions_h_


// Library includes

// Project includes
#include <Common/Exceptions.h>

// Forward declarations

// Namespace declarations


namespace JSON {
namespace Exceptions {


class JSONException : public ::Common::Exceptions::Exception
{
public:
	JSONException(const std::string& text)
	: ::Common::Exceptions::Exception(text)
	{}
	virtual ~JSONException() { }
};


class DuplicateKey : public JSONException
{
public:
	DuplicateKey(const std::string& text)
	: JSONException(text)
	{ }
};


class InvalidKey : public JSONException
{
public:
	InvalidKey(const std::string& text)
	: JSONException(text)
	{ }
};

class UnknownKey : public JSONException
{
public:
	UnknownKey(const std::string& text)
	: JSONException(text)
	{ }
};


}
}


#endif
