
#ifndef _ODBC_SQLResult_h_
#define _ODBC_SQLResult_h_


// Library includes
#include <string>
#include <vector>

// Project includes
#include "SQLRecord.h"

// Forward declarations

// Namespace declarations


namespace Database {


class SQLResult
{
public:
	SQLResult();
	~SQLResult();

	SQLRecord operator[] (unsigned int idx);

	void add(const SQLRecord& record);
	bool empty() const;
	size_t size() const;

	bool next();
	SQLRecord value() const;

protected:

private:
	std::vector<SQLRecord>::iterator	mIterator;
	std::vector<SQLRecord>				mRecords;
};


}


#endif
