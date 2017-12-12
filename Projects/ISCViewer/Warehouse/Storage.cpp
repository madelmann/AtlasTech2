
// Header
#include "Storage.h"

// Library includes
#include <sqlite3\include\sqlite3.h>

// Project includes

// Namespace declarations
using Database::SQLEntry;
using Database::SQLRecord;
using Database::SQLResult;


namespace WarehouseSpace {


static SQLResult QueryResult;

static int callback(void * /*NotUsed*/, int argc, char **argv, char **azColName)
{
	SQLRecord record;

	for ( int i = 0; i < argc; i++ ) {
		SQLEntry entry;
		entry.Name = azColName[i];
		entry.Value = std::string(argv[i] ? argv[i] : "NULL");

		record.add(entry);
	}

	QueryResult.add(record);
	return 0;
}


Storage::Storage(const Common::Logger *p)
: Common::Logger(p, "Storage"),
  mDatabase(0)
{
	debug("Starting...");
}

Storage::~Storage()
{
	debug("Stopping...");

	close();
}

void Storage::close()
{
	if ( mDatabase ) {
		sqlite3_close(mDatabase);
	}
}

bool Storage::open(const std::string &filename)
{
	if ( filename.empty() ) {
		error("invalid filename provided!");
		return false;
	}

	int rc = sqlite3_open(filename.c_str(), &mDatabase);
	if ( rc ) {
		error("error while opening database '" + filename + "'!");
		sqlite3_close(mDatabase);
		return false;
	}

	return true;
}

const Database::SQLResult& Storage::query(const std::string str)
{
	// reset our query result
	QueryResult = SQLResult();

	char *zErrMsg = 0;

	int rc = sqlite3_exec(mDatabase, str.c_str(), callback, 0, &zErrMsg);
	if ( rc != SQLITE_OK ) {
		error("SQL error: " + std::string(zErrMsg));
		sqlite3_free(zErrMsg);
	}

	return QueryResult;
}


}
