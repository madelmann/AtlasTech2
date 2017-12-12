
#ifndef JSON_JsonConnection_h
#define JSON_JsonConnection_h


// Library includes
#include <fstream>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Common/IDatabaseConnection.h>
#include "JsonPhrase.h"

// Forward declarations

// Namespace declarations


namespace JSON {


class JsonConnection : public Common::IDatabaseConnection,
					   private Common::Logger
{
public:
	JsonConnection(const Common::Logger *p);
	~JsonConnection();

public:
	// Common::IDatabaseConnection implementation
	// {
	void close();
	void open(const std::string& database, const std::string& user = "", const std::string& password = "");
	// }

	void begin();
	void commit();
	void rollback();

	JsonPhrase read();
	void write(const JsonPhrase& data);

protected:

private:
	void reset();

private:
	bool			mActiveTransaction;
	std::string		mDatabase;
	std::fstream	mDataStorage;
	JsonPhrase		mReadData;
	JsonPhrase		mWriteData;
};


}


#endif
