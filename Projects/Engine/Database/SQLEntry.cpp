
// Header
#include "SQLEntry.h"

// Library includes

// Project includes

// Namespace declarations


namespace Database {


const char* SQLEntry::Null = "<NULL>";


SQLEntry::SQLEntry()
: mDataType(0),
  mDecimalDigits(false),
  mNullable(false),
  mSize(0)
{
}

SQLEntry::~SQLEntry()
{
}

std::string SQLEntry::toString() const
{
	return "[\"" + Name + "\" : \"" + Value + "\"]";
}

}
