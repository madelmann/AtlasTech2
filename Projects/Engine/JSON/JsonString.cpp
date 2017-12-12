
// Header
#include "JsonString.h"

// Library includes

// Project includes
#include "Exceptions.h"

// Namespace declarations


namespace JSON {


JsonString::JsonString()
: JsonValue("", "", "")
{
}

JsonString::JsonString(const std::string& name, const std::string& type, const std::string& value)
: JsonValue(name, type, value)
{
}

JsonString& JsonString::operator[] (const std::string& key)
{
	for ( std::list<JsonString>::iterator it = mStrings.begin(); it != mStrings.end(); it++ ) {
		if ( Tools::StringCompare((*it).getName(), key) ) {
			return (*it);
		}
	}

	throw Exceptions::UnknownKey(
		std::string("key '" + key + "' not found!").c_str()
	);
}

void JsonString::add(JsonString jstring)
{
	std::string name = jstring.getName();

	for ( JsonStrings::const_iterator it = mStrings.begin(); it != mStrings.end(); ++it ) {
		if ( (*it).getName() == name ) {
			throw Exceptions::DuplicateKey("duplicate key added: " + name);
		}
	}

	mStrings.push_back(jstring);
}

void JsonString::clear()
{
	mStrings.clear();
}

JsonString::JsonStrings JsonString::getValue() const
{
	return mStrings;
}

bool JsonString::isValid() const
{
	return ( mName.length() > 0 ) || ( mType.length() > 0 ) || ( mValue.length() > 0 ) || ( mStrings.size() > 0 );
}

void JsonString::setType(const std::string& type)
{
	mType = type;
}

void JsonString::setValue(const std::string& value)
{
	mValue = value;
}

void JsonString::setValue(const JsonStrings &value)
{
	mStrings = value;
}

std::string JsonString::toString() const
{
	std::string result;
	if ( ofType("object") ) {
		result += "{";
		if ( mStyledString )
			result += "\n";
		else
			result += " ";
	}

	std::list<JsonString>::const_iterator it = mStrings.begin();
	while ( it != mStrings.end() ) {
		if ( mStyledString )
			result += "\t";

		if ( (*it).ofType("object") ) {
			result += "\"" + (*it).getName() + "\": ";
			result += (*it).toString();
		}
		else {
			result += ((JsonValue)(*it)).toString();
		}

		it++;

		if ( it != mStrings.end() ) {
			result += ",";
		}

		if ( mStyledString )
			result += "\n";
		else
			result += " ";
	}

	if ( ofType("object") ) {
		result += "}";
	}

	return result;
}

std::string JsonString::toStyledString()
{
	mStyledString = true;

	std::string result = toString();

	mStyledString = false;

	return result;
}


}
