
// Header
#include "Connection.h"

// Library includes
#include <iostream>

// Project includes
#include <Tools/Files.h>
#include "Exceptions.h"
#include "Parser.h"
#include "Writer.h"

// Namespace declarations


namespace Json {


Connection::Connection(const Common::Logger *p)
: Common::Logger(p, "Connection"),
  mActiveTransaction(false)
{
}

Connection::~Connection()
{
	close();
}

void Connection::begin()
{
	debug("begin()");

	if ( mActiveTransaction ) {
		// a transaction is already active => throw an error
		error("starting a transaction while another one is still active!");
		throw Exceptions::JSONException("starting a transaction while another one is still active!");
	}

	if ( mDatabase.empty() ) {
		// no active database connection => throw an error
		error("starting a transaction without an active data storage connection!");
		throw Exceptions::JSONException("starting a transaction without an active data storage connection!");
	}

	mActiveTransaction = true;
}

void Connection::commit()
{
	debug("commit()");

	if ( mDatabase.empty() ) {
		error("trying to write without an active data storage connection!");
		throw Exceptions::JSONException("trying to write without an active data storage connection!");
	}

	StyledWriter writer;
	std::string data = writer.toString(mWriteData);

	mDataStorage.open(mDatabase.c_str(), std::ios::out);
	mDataStorage.write(data.c_str(), data.size());		// write data to file
	mDataStorage.close();

	// after a successful commit, we skip re-reading our database and just reset our "loaded" data
	mReadData = mWriteData;

	mActiveTransaction = false;
}

void Connection::close()
{
	if ( !isOpen() ) {
		// no connection is open => return silent
		return;
	}

	if ( mActiveTransaction ) {
		// we are closing our data storage connection although a transaction is active => throw an error
		error("closing data storage connection during active transaction!");
		throw Exceptions::JSONException("closing data storage connection during active transaction!");
	}

	debug("Disconnecting from file based storage '" + mDatabase + "'...");

	reset();
}

void Connection::open(const std::string& database, const std::string& user, const std::string& /*password*/)
{
	mDatabase = database;

	debug("Connecting to file based storage ('" + mDatabase + "')...");
	if ( !user.empty() ) {
		debug("Using credentials (User: " + user + ", Password: <hidden>)");
	}

	if ( !Tools::Files::exists(database) ) {
		warn("File '" + mDatabase + "' does not exist. Creating...");
	}

	mDataStorage.open(mDatabase.c_str(), std::ios::in);
	std::string data((std::istreambuf_iterator<char>(mDataStorage)), std::istreambuf_iterator<char>());

	mDataStorage.close();

	mReadData = Parser::parse(data);
	mWriteData = mReadData;

	mIsActive = true;
}

Value Connection::read()
{
	debug("Reading data from storage ('" + mDatabase + "')...");

	return mReadData;
}

void Connection::reset()
{
	mDatabase = "";
	mReadData = Value();
	mWriteData = Value();

	mIsActive = false;
}

void Connection::rollback()
{
	error("rollback()");

	if ( mDatabase.empty() ) {
		error("trying to write without datastore connection!");
		throw Exceptions::JSONException("trying to write without datastore connection!");
	}

	// reset all written data to what we have initially read
	mWriteData = mReadData;

	mActiveTransaction = false;
}

void Connection::write(const Value& data)
{
	debug("Writing data to storage ('" + mDatabase + "')...");

	mWriteData = data;
}


}
