
// Header
#include "ConfigParser.h"

// Library includes

// Project includes
#include <Tools/Tools.h>

// Namespace declarations


ConfigParser::ConfigParser(const Common::Logger *p)
: Common::Logger(p, "ConfigParser"),
  mError(0),
  mFile(0)
{
}

ConfigParser::~ConfigParser()
{
	close();
}

void ConfigParser::close()
{
	if ( mFile ) {
		fclose(mFile);
	}
}

Value ConfigParser::getValue(const std::string& item)
{
	char line[255] = "";
	char result[255] = "";

	open();

	do {
		sprintf(line, "");
		Tools::readstr(mFile, line);

		char str[255];
		sscanf(line, "%s\0", str);

		if ( strcmpi(str, item.c_str()) == 0 ) {
			sscanf(line, "%s %s", item.c_str(), &result);
			close();
			return Value(item, "undefined", result);
		}

	} while ( strcmp(line, "") != 0 );

	close();
	return Value("");
}

std::string ConfigParser::readLine()
{
	char line[255];

	sprintf(line, "");
	Tools::readstr(mFile, line);

	return std::string(line);
}

bool ConfigParser::load(const std::string& filename)
{
	debug("load('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid file name provided!");
		return false;
	}

	fopen_s(&mFile, filename.c_str(), "rt");

	if ( !mFile ) {
		warn("could not load configuration file '" + filename + "'!");
		return false;
	}

	fclose(mFile);

	mFilename = filename;

	return true;
}

void ConfigParser::open()
{
	fopen_s(&mFile, mFilename.c_str(), "rt");
}
