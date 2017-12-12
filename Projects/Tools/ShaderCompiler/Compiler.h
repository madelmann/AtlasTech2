
#ifndef _ShaderCompiler_Compiler_h_
#define _ShaderCompiler_Compiler_h_


// Library includes
#include <string>

// Project includes
#include <Common/Logger.h>

// Forward declarations

// Namespace declarations


class Compiler : private Common::Logger
{
public:
	Compiler(const Common::Logger *p);

	void init();
	int process(const std::string& filename);

protected:

private:

};


#endif
