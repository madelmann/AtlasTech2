
#ifndef JsonNG_Storage_h
#define JsonNG_Storage_h


// Library includes

// Project includes
#include <Common/Logger.h>
#include "Transaction.h"
#include "Value.h"

// Forward declarations

// Namespace declarations

namespace Json {

// Forward declarations
class Connection;

class Storage : private Common::Logger
{
public:
	Storage(const Common::Logger *p);
	~Storage();

public:
	Transaction::Ptr createTransaction();

	void load(const std::string& filename, Value& value);
	void store(const Value& value);

protected:

private:
	Connection	*mConnection;
};


}


#endif
