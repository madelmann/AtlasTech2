
#ifndef Interfaces_Common_AComponent_h
#define Interfaces_Common_AComponent_h


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {


class AComponent
{
public:
	virtual ~AComponent() { }

public:
	virtual void configurationComplete() = 0;
	virtual void shutdown() = 0;

public:
	bool isReady() const {
		return mIsReady;
	}

protected:
	AComponent()
	: mConfigurationComplete(false),
	  mIsReady(false)
	{ }

protected:
	bool	mConfigurationComplete;
	bool	mIsReady;
};


}


#endif
