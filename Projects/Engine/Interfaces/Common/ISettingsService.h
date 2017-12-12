
#ifndef _Interface_Common_ISettingsService_h_
#define _Interface_Common_ISettingsService_h_


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {


class ISettingsService
{
public:
	virtual ~ISettingsService() { }

public:
	virtual void clear() = 0;

public:
	bool hasChanged() {
		bool result = mHasChanged;

		// reset after reading
		mHasChanged = false;

		return result;
	}

protected:
	void changed() {
		mHasChanged = true;
	}
	void reset() {
		mHasChanged = false;
	}

private:
	bool mHasChanged;
};


}


#endif
