
#ifndef Interfaces_Manager_ITextureLoader_h
#define Interfaces_Manager_ITextureLoader_h


// Library includes
#include <cassert>

// Project includes
#include <Core/EngineSettings.h>
#include <Tools/Files.h>
#include <Tools/Strings.h>

// Forward declaration

// Namespace declarations


namespace TextureSpace {

// Forward declarations
class Texture;

class ATextureLoader
{
public:
	virtual ~ATextureLoader() { }

public:
	virtual Texture* create(const std::string& filename) = 0;

public:
	void connectEngineSettings(Core::EngineSettings *s) {
		assert(s);
		assert(!mEngineSettings);

		mEngineSettings = s;
	}

	bool handleFormat(const std::string& format) const {
		return Tools::StringCompareI(mFormat, format);
	}

protected:
	ATextureLoader(const std::string& format)
	: mEngineSettings(0),
	  mFormat(format)
	{ }

protected:
	std::string	mFormat;

	Core::EngineSettings	*mEngineSettings;
};


}


#endif
