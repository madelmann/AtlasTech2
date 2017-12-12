
#ifndef JsonNG_Transaction_h
#define JsonNG_Transaction_h


// Library includes

// Project includes
#include <Interfaces/Common/Transaction.h>
#include "Types.h"

// Forward declarations

// Namespace declarations


namespace Json {

// Forward declarations
class Connection;

class Transaction : public Common::Transaction
{
public:
	typedef SmartPtr<Transaction> Ptr;

public:
	Transaction(Connection *connection);
	~Transaction();

public:
	void begin();
	void commit();
	void rollback();

protected:

private:
	Connection	*mConnection;
	bool		mIsActive;
};


}


#endif
