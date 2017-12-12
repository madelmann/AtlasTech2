
#ifndef Interfaces_Core_IController_h
#define Interfaces_Core_IController_h


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Core {


class IController
{
public:
	virtual ~IController() { }

public:
	bool isRunning() const {
		return mIsRunning;
	}

	virtual void Pause() = 0;
	virtual void Play() = 0;

protected:
	IController()
	: mIsRunning(false)
	{ }

protected:
	bool mIsRunning;

private:

};


}


#endif
