
#ifndef ObjecttiveScript_BuildInObjects_Bool_h
#define ObjecttiveScript_BuildInObjects_Bool_h


// Library includes

// Project includes
#include <ObjectiveScript/Object.h>

// Forward declarations

// Namespace declarations


namespace ObjectiveScript {


class Bool : public Object
{
public:
	Bool()
	: Object("", "SYSTEM.OS", "Bool", "")
	{ }

	Bool(const std::string& value)
	: Object("", "SYSTEM.OS", "Bool", value)
	{ }

public:
	void setTokens(const TokenList& tokens) {
		(void)tokens;
	}

protected:

private:

};


}


#endif
