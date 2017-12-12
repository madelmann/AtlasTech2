
// Header
#include "XMLParser.h"

// Library ioncludes

// Project includes

// Namespace declarations


XMLParser::XMLParser(const Logger *p)
: Logger(p, "XMLParser")
{
}

XMLParser::~XMLParser()
{
}

std::string XMLParser::getValue(const std::string& /*item*/)
{
	return std::string();
}

bool XMLParser::load(const std::string& /*filename*/)
{
	return false;
}

