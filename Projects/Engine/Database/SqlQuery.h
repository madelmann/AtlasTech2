
#ifndef ODBC_SqlQuery_h
#define ODBC_SqlQuery_h


// Library includes
#include <string>

// Project includes
#include "SQLResult.h"
#include <Parsers/Value.h>

// Forward declarations

// Namespace declarations


namespace Database {

// Forward declarations
class IConnection;

class SqlQuery
{
public:
	SqlQuery(IConnection *driver);

	void bind(const std::string& key, const Value& value);
	void exec();
	const std::string& executedQuery() const;
	bool next();
	void prepare(const std::string& query);
	SQLRecord record() const;
	const SQLResult& result() const;

protected:

private:
	std::string escape(const std::string& str);

	IConnection	*mDriver;
	std::string	mExecutedQuery;
	std::string	mQuery;
	SQLResult	mResult;
};


}


#endif
