
#ifndef Interfaces_Common_Transaction_h
#define Interfaces_Common_Transaction_h


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {


class Transaction
{
public:
	virtual ~Transaction() { }

public:
	virtual void begin() = 0;
	virtual void commit() = 0;
	virtual void rollback() = 0;
};


}


#endif
