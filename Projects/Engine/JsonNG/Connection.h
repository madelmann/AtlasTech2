
#ifndef JsonNG_Connection_h
#define JsonNG_Connection_h


// Library includes
#include <fstream>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Common/IDatabaseConnection.h>
#include "Value.h"

// Forward declarations

// Namespace declarations


namespace Json {


class Connection : public Common::IDatabaseConnection,
				   private Common::Logger
{
public:
	Connection(const Common::Logger *p);
	~Connection();

public:
	// Common::IDatabaseConnection implementation
	// {
	void close();
	void open(const std::string& database, const std::string& user = "", const std::string& password = "");
	// }

	void begin();
	void commit();
	void rollback();

	Value read();
	void write(const Value& data);

protected:

private:
	void reset();

private:
	bool			mActiveTransaction;
	std::string		mDatabase;
	std::fstream	mDataStorage;
	Value			mReadData;
	Value			mWriteData;
};


}


#endif
