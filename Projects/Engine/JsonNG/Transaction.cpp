
// Header
#include "Transaction.h"

// Library includes
#include <cassert>

// Project includes
#include "Connection.h"

// Namespace declarations


namespace Json {


Transaction::Transaction(Connection *connection)
: mConnection(connection),
  mIsActive(false)
{
}

Transaction::~Transaction()
{
	if ( mIsActive ) {
		rollback();
	}

	mIsActive = false;
}

void Transaction::begin()
{
	assert(mConnection);
	mConnection->begin();

	mIsActive = true;
}

void Transaction::commit()
{
	assert(mConnection);
	mConnection->commit();

	mIsActive = false;
}

void Transaction::rollback()
{
	assert(mConnection);
	mConnection->rollback();

	mIsActive = false;
}


}
