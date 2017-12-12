
#ifndef Interfaces_Common_IDatabaseConnection_h
#define Interfaces_Common_IDatabaseConnection_h


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {


class IDatabaseConnection
{
public:
	virtual ~IDatabaseConnection() { }

public:
	virtual void close() = 0;
	virtual void open(const std::string& database, const std::string& user, const std::string& password) = 0;

public:
	bool isOpen() const {
		return mIsActive;
	}

protected:
	IDatabaseConnection()
	: mIsActive(false)
	{ }

protected:
	bool	mIsActive;

private:

};


}


#endif
