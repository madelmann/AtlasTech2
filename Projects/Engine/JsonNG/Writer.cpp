
// Header
#include "Writer.h"

// Library includes

// Project includes

// Namespace declarations


namespace Json {


Writer::Writer()
: mWhiteSpaceEnd(" "),
  mWhiteSpaceIndent(""),
  mWhiteSpaceStart(" ")
{
}

std::string Writer::indent(unsigned int num)
{
	std::string result;

	for ( unsigned int i = 0; i < num; i++ ) {
		result += mWhiteSpaceIndent;
	}

	return result;
}

std::string Writer::toString(const Value& root, unsigned int indents)
{
	std::string result;
	if ( indents == 0 && root.isObject() ) {
		result += "{" + mWhiteSpaceStart;
	}

	Value::Members members = root.members();

	Value::Members::const_iterator it = members.begin();
	while ( it != members.end() ) {
		result += indent(indents);
		if ( root.isObject() ) {
			result += indent(1);
		}

		result += "\"";
		result += (*it).first;
		result += "\": ";

		Value v = (*it).second;
		switch ( v.type() ) {
			case Value::Type::NIL:
				result += "null";
				break;
			case Value::Type::BOOL:
			case Value::Type::DOUBLE:
			case Value::Type::FLOAT:
			case Value::Type::INT:
			case Value::Type::STRING:
			case Value::Type::UINT:
			case Value::Type::UNDEFINED:
				if ( !v.isArrayElement() )
					result += v.value();
				break;
			case Value::Type::ARRAY:
				if ( !v.size() ) {
					result += "[ ]" + mWhiteSpaceEnd;
				}
				else {
					result += "[" + mWhiteSpaceStart;

					Value::Members members = v.members();
					Value::Members::const_iterator it = members.begin();

					while ( it != members.end() ) {
						result += indent(indents + 2);
						result += (*it).second.value();
						it++;

						if ( it != members.end() ) {
							result += "," + mWhiteSpaceEnd;
						}
					}

					result += mWhiteSpaceStart + indent(indents + 1) + "]";
				}
				break;
			case Value::Type::OBJECT:
				if ( !v.size() ) {
					result += "{ }" + mWhiteSpaceEnd;
				}
				else {
					result += "{" + mWhiteSpaceStart;
					result += toString(v, indents + 1);
					result += mWhiteSpaceStart + indent(indents + 1) + "}";
				}
				break;
		}

		it++;

		if ( it != members.end() ) {
			result += "," + mWhiteSpaceEnd;
		}
	}

	if ( indents == 0 && root.isObject() ) {
		result += mWhiteSpaceEnd + "}";
	}
	return result;
}


StyledWriter::StyledWriter()
{
	mWhiteSpaceEnd = "\n";
	mWhiteSpaceIndent = "    ";
	mWhiteSpaceStart = "\n";
}


}
