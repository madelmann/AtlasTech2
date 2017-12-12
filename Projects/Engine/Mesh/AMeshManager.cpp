
// Header
#include "AMeshManager.h"

// Library includes

// Project includes
#include <Tools/Strings.h>

// Namespace declarations


namespace Mesh {


AMeshManager::AMeshManager(const Common::Logger *p, const std::string& name)
: Common::Logger(p, name)
{
}

bool AMeshManager::handleFormat(const std::string& format) const
{
	return Tools::StringCompareI(mFormat, format);
}

void AMeshManager::init()
{
	debug("Initializing...");

	mIsReady = true;
}

void AMeshManager::manage()
{
	// nothing to do here
}

void AMeshManager::shutdown()
{
	debug("Stopping...");

	clear();
}

void AMeshManager::update(float /*elapsedTime*/)
{
	// nothing to do
}


}
