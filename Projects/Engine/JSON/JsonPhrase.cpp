
// Header
#include "JsonPhrase.h"

// Library includes
#include <list>

// Project includes

// Namespace declarations


namespace JSON {


std::string JsonPhrase::toString()
{
	std::string result;
	result += "{";

	if ( mStyledString )
		result += "\n";
	else
		result += " ";

	result += JsonString::toString();

	result += "}";
	return result;
}

std::string JsonPhrase::toStyledString()
{
	mStyledString = true;

	std::string result = toString();

	mStyledString = false;

	return result;
}


}
