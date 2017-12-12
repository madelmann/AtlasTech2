
#ifndef JSON_JsonTransaction_h
#define JSON_JsonTransaction_h


// Library includes

// Project includes
#include <Interfaces/Common/Transaction.h>
#include "Types.h"

// Forward declarations

// Namespace declarations


namespace JSON {

// Forward declarations
class JsonConnection;

class JsonTransaction : public Common::Transaction
{
public:
	typedef SmartPtr<JsonTransaction> Ptr;

public:
	JsonTransaction(JsonConnection *connection);
	~JsonTransaction();

public:
	void begin();
	void commit();
	void rollback();

protected:

private:
	JsonConnection	*mConnection;
	bool			mIsActive;
};


}


#endif
