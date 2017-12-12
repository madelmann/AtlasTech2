
#ifndef _ODBC_SQLEntry_h_
#define _ODBC_SQLEntry_h_


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace Database {


class SQLEntry
{
public:
	SQLEntry();
	~SQLEntry();

	std::string toString() const;

	std::string	Name;
	std::string	Value;

	int	mDataType;
	int	mDecimalDigits;
	int	mNullable;
	int	mSize;

	static const char* Null;

protected:

private:

};


}


#endif
