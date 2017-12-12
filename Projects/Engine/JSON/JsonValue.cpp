
// Header
#include "JsonValue.h"

// Library includes

// Project includes

// Namespace delarations


namespace JSON {


JsonValue::JsonValue(const std::string& name, const std::string& type, const std::string& value)
: Value(name, type, value),
  mStyledString(false)
{
}

std::string JsonValue::toString() const
{
	std::string result;
	result += "\"" + getName() + "\": ";

	if ( ofType("string") ) {
		result += "\"";
	}

	result += getValue();

	if ( ofType("string") ) {
		result += "\"";
	}

	return result;
}

std::string JsonValue::toStyledString()
{
	mStyledString = true;

	std::string result = toString();

	mStyledString = false;

	return result;
}


}
