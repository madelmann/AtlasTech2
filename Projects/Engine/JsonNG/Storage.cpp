
// Header
#include "Storage.h"

// Library includes

// Project includes
#include <Tools/Files.h>
#include "Connection.h"
#include "Exceptions.h"
#include "Parser.h"

// Namespace declarations


namespace Json {


Storage::Storage(const Common::Logger *p)
: Common::Logger(p, "Storage"),
  mConnection(0)
{
	debug("Starting...");

	mConnection = new Connection(this);
}

Storage::~Storage()
{
	debug("Stopping...");

	if ( mConnection ) {
		mConnection->close();

		delete mConnection;
		mConnection = 0;
	}
}

Transaction::Ptr Storage::createTransaction()
{
	Transaction::Ptr t = Transaction::Ptr(
		new Transaction(mConnection)
	);

	t.get()->begin();

	return t;
}

void Storage::load(const std::string& filename, Value& value)
{
	//debug("Loading data from file '" + filename + "'...");

	// Close connection if any is open
	if ( mConnection && mConnection->isOpen() ) {
		mConnection->close();
	}

	mConnection->open(filename, "", "");
	if ( !mConnection->isOpen() ) {
		error("could not open data storage!");
		return;
	}

	value = mConnection->read();
}

void Storage::store(const Value& value)
{
	//debug("Storing Json::Value to file...");

	try {
		mConnection->begin();

		mConnection->write(value);

		mConnection->commit();
	}
	catch ( Exceptions::JSONException &e ) {
		error(e.what());

		mConnection->rollback();
	}

	//Transaction::Ptr t = createTransaction();

	//mConnection->write(phrase);

	//t.get()->commit();
}


}
