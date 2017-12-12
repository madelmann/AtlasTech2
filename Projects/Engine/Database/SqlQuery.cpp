
// Header
#include "SqlQuery.h"

// Library includes

// Project includes
#include "Exceptions.h"
#include <Interfaces/ODBC/IConnection.h>

// Namespace declarations


namespace Database {


SqlQuery::SqlQuery(IConnection *driver)
: mDriver(driver)
{
}

void SqlQuery::bind(const std::string& key, const Value& value)
{
	//size_t found = mQuery.find(key);

	//if ( found == std::string::npos ) {
	//	throw Exceptions::InvalidBind(key);
	//}

	size_t found;

	while ( (found = mQuery.find(key)) != std::string::npos ) {
		if ( value.getType() == "string" ) {
			mQuery.replace(found, key.length(), "'" + escape(value.toString()) + "'");
		}
		else {
			mQuery.replace(found, key.length(), value.toString());
		}
	}
}

std::string SqlQuery::escape(const std::string& str)
{
	std::string result;

	for ( std::string::const_iterator it = str.begin(); it != str.end(); ++it ) {
		switch ( (*it) ) {
			case '\'':
				result += "\\\'";
				break;
			case '"':
				result += "\\\"";
				break;
			case '\\':
				result += "\\\\";
				break;
			default:
				result += (*it);
				break;
		}
	}

	return result;
}

void SqlQuery::exec()
{
	mExecutedQuery = mQuery;

	mResult = mDriver->query(mQuery);
}

const std::string& SqlQuery::executedQuery() const
{
	return mExecutedQuery;
}

bool SqlQuery::next()
{
	return mResult.next();
}

SQLRecord SqlQuery::record() const
{
	return mResult.value();
}

const SQLResult& SqlQuery::result() const
{
	return mResult;
}

void SqlQuery::prepare(const std::string& query)
{
	mQuery = query;
}


}
