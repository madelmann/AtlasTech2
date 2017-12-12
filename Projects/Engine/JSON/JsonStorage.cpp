
// Header
#include "JsonStorage.h"

// Library includes

// Project includes
#include <Tools/Files.h>
#include "Exceptions.h"
#include "JsonConnection.h"
#include "Parser.h"

// Namespace declarations


namespace JSON {


JsonStorage::JsonStorage(const Common::Logger *p)
: Common::Logger(p, "JsonStorage"),
  mConnection(0)
{
	debug("Starting...");

	mConnection = new JsonConnection(this);
}

JsonStorage::~JsonStorage()
{
	debug("Stopping...");

	if ( mConnection ) {
		delete mConnection;
		mConnection = 0;
	}
}

JsonTransaction::Ptr JsonStorage::createTransaction()
{
	JsonTransaction::Ptr t = JsonTransaction::Ptr(
		new JsonTransaction(mConnection)
	);

	t.get()->begin();

	return t;
}

void JsonStorage::load(const std::string& filename, JsonPhrase& phrase)
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

	phrase = mConnection->read();
}

void JsonStorage::store(const JsonPhrase& phrase)
{
	//debug("Storing JsonPhrase to file...");

	try {
		mConnection->begin();

		mConnection->write(phrase);

		mConnection->commit();
	}
	catch ( Exceptions::JSONException &e ) {
		error(e.what());

		mConnection->rollback();
	}

	//JsonTransaction::Ptr t = createTransaction();

	//mConnection->write(phrase);

	//t.get()->commit();
}


}
