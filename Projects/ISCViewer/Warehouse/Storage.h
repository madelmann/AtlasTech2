
#ifndef Warehouse_Storage_h
#define Warehouse_Storage_h


// Library includes
#include <string>

// Project includes
#include <Common\Logger.h>
#include <Database\SQLResult.h>

// Forward declarations
struct sqlite3;

// Namespace declarations


namespace WarehouseSpace {


class Storage : private Common::Logger
{
public:
	Storage(const Common::Logger *p);
	~Storage();

public:
	void close();
	bool open(const std::string &filename);

	const Database::SQLResult& query(const std::string str);

protected:

private:
	sqlite3	*mDatabase;
};


}


#endif
