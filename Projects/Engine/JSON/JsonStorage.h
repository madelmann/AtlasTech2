
#ifndef JSON_JsonStorage_h
#define JSON_JsonStorage_h


// Library includes

// Project includes
#include <Common/Logger.h>
#include "JsonPhrase.h"
#include "JsonTransaction.h"

// Forward declarations

// Namespace declarations

namespace JSON {

// Forward declarations
class JsonConnection;


class JsonStorage : private Common::Logger
{
public:
	JsonStorage(const Common::Logger *p);
	~JsonStorage();

public:
	JsonTransaction::Ptr createTransaction();

	void load(const std::string& filename, JsonPhrase& phrase);
	void store(const JsonPhrase& phrase);

protected:

private:
	JsonConnection	*mConnection;
};


}


#endif
