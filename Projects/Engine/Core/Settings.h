
#ifndef Core_Settings_h
#define Core_Settings_h


// Library includes

// Project includes
#include <Interfaces/Common/ISettingsService.h>

// Forward declarations

// Namespace declarations


namespace Core {


class Settings : public Common::ISettingsService
{
public:
	Settings() {
		clear();
	}

public:
	void clear() {
		mUseVertexBufferObjects = false;
	}

	bool useVertexBufferObjects() const {
		return mUseVertexBufferObjects;
	}
	void useVertexBufferObjects(bool state) {
		mUseVertexBufferObjects = state;
	}

private:
	bool mUseVertexBufferObjects;
};


}


#endif
