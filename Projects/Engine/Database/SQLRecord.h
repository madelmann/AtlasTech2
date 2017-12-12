
#ifndef _ODBC_SQLRecord_h_
#define _ODBC_SQLRecord_h_


// Library includes
#include <string>
#include <vector>

// Project includes
#include "SQLEntry.h"

// Forward declarations

// Namespace declarations


namespace Database {

// Forward declarations
class SQLEntry;

class SQLRecord
{
public:
	SQLRecord();
	~SQLRecord();

	SQLEntry operator[] (unsigned int idx);
	SQLEntry operator[] (const std::string& column);

	void add(const SQLEntry& entry);
	bool empty() const;
	bool hasNext() const;
	void next();
	size_t size() const;
	SQLEntry value();

protected:

private:
	std::vector<SQLEntry>			mEntries;
	std::vector<SQLEntry>::iterator	mIterator;
};


}


#endif
