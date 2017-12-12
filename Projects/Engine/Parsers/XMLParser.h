
#ifndef _Parsers_XMLParser_h_
#define _Parsers_XMLParser_h_


// Library includes

// Project includes
#include <Common/Logger.h>

// Forward declarations

// Namespace declarations
using Common::Logger;


class XMLParser : private Logger
{
public:
	XMLParser(const Logger *p);
	~XMLParser();

public:
	std::string getValue(const std::string& item);
	bool load(const std::string& filename);

protected:

private:

};


#endif
