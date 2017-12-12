
#ifndef _Interfaces_Console_IConsolePrinter_h_
#define _Interfaces_Console_IConsolePrinter_h_


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace Console {


class IPrinter
{
public:
	virtual ~IPrinter() { }

public:
	virtual void clear() = 0;
	virtual void print(const std::string& text) = 0;
	virtual void print(const std::string& text, int line, int column) = 0;
};


}


#endif
