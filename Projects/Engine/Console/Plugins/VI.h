
#ifndef _Console_Plugins_VI_h_
#define _Console_Plugins_VI_h_


// Library includes

// Project includes
#include <Interfaces/Console/IConsoleProgram.h>

// Forward declarations

// Namespace declarations


namespace Console {


class VI : public IConsoleProgram
{
public:
	VI();
	~VI();

	void processKeyPress(WPARAM key);

protected:

private:
	enum eMode {
		InsertMode = 0,
		VisualMode
	};

	void switchMode(eMode mode);

	eMode	mMode;
};


}


#endif
