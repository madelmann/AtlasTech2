
#ifndef _Parsers_ConfigParser_h_
#define _Parsers_ConfigParser_h_


// Library includes
#include <string>

// Project includes
#include "Value.h"
#include <Common/Logger.h>

// Forward declarations

// Namespace declarations


class ConfigParser : public Common::Logger
{
public:
	ConfigParser(const Common::Logger *p);
	~ConfigParser();

public:
	Value getValue(const std::string& item);
	bool load(const std::string& filename);

protected:

private:
	void close();
	void open();
	std::string readLine();

private:
	int			mError;
	FILE		*mFile;
	std::string	mFilename;
};


#endif
