
// Header
#include "Configurator.h"

// Library includes

// Project includes
#include <Parsers/LineParser.h>
#include <Tools/Tools.h>

// Namespace declarations


namespace Core {
namespace Configuration {


Configurator::Configurator(const Common::Logger *p, const std::string& file)
: Common::Logger(p, "Configurator"),
  mConfigFile(file),
  mFile(0),
  mSuccess(false)
{
	if ( !file.empty() ) {
		load(file);
	}
}

Configurator::~Configurator()
{
	unload();
}

std::string Configurator::consumeLine()
{
	char line[255];

	sprintf(line, "");
	Tools::readstr(mFile, line);

	return std::string(line);
}

Value Configurator::getValue(const std::string& key, int idx) const
{
	std::string member = key;
	if ( idx != -1 ) {
		member += ":" + Tools::toString(idx);
	}

	KeyValueMap::const_iterator it;
	for ( it = mValues.begin(); it != mValues.end(); ++it ) {
		if ( Tools::StringCompare(it->first, member) ) {
			return it->second;
		}
	}

	//error("Key '" + member + "' not found in configuration file '" + mConfigFile + "'!");
	return Value();
}

Value Configurator::getValue(const std::string& parent, int idx, const std::string& key) const
{
	return getValue(parent + ":" + Tools::toString(idx) + "." + key);
}

bool Configurator::isMember(const std::string& key, int idx) const
{
	std::string member = key;
	if ( idx != -1 ) {
		member += ":" + Tools::toString(idx);
	}

	KeyValueMap::const_iterator it;
	for ( it = mValues.begin(); it != mValues.end(); ++it ) {
		if ( Tools::StringCompare(it->first, member) ) {
			return true;
		}
	}

	//error("Key '" + member + "' not found in configuration file '" + mConfigFile + "'!");
	return false;
}

bool Configurator::isMember(const std::string& parent, int idx, const std::string& key) const
{
	return isMember(parent + ":" + Tools::toString(idx) + "." + key);
}

bool Configurator::load(const std::string& file)
{
	debug("load('" + file + "')");

	unload();

	mConfigFile = file;
	if ( mConfigFile.empty() ) {
		warn("invalid file name provided!");
		return mSuccess;
	}

	fopen_s(&mFile, file.c_str(), "rt");

	if ( !mFile ) {
		warn("could not load configuration file '" + mConfigFile + "'!");
		return mSuccess;
	}

	split();

	mSuccess = true;
	return mSuccess;
}

void Configurator::split()
{
	mValues.clear();

	std::string line;
	do {
		line = consumeLine();

		Value value = LineParser::parse(line);

		if ( !value.getName().empty() ) {
			mValues[value.getName()] = value;
		}
	} while ( !line.empty() );

	rewind(mFile);
}

bool Configurator::success() const
{
	return mSuccess;
}

void Configurator::unload()
{
	mConfigFile = "";
	mSuccess = false;
	mValues.clear();

	if ( mFile ) {
		fclose(mFile);
		mSuccess = true;
	}
}


}
}
