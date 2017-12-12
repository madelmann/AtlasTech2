
// Header
#include "JsonTransaction.h"

// Library includes
#include <cassert>

// Project includes
#include "JsonConnection.h"

// Namespace declarations


namespace JSON {


JsonTransaction::JsonTransaction(JsonConnection *connection)
: mConnection(connection),
  mIsActive(false)
{
}

JsonTransaction::~JsonTransaction()
{
	if ( mIsActive ) {
		rollback();
	}

	mIsActive = false;
}

void JsonTransaction::begin()
{
	assert(mConnection);
	mConnection->begin();

	mIsActive = true;
}

void JsonTransaction::commit()
{
	assert(mConnection);
	mConnection->commit();

	mIsActive = false;
}

void JsonTransaction::rollback()
{
	assert(mConnection);
	mConnection->rollback();

	mIsActive = false;
}


}
