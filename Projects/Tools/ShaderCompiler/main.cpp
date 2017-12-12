
#pragma once
#pragma warning(disable : 4068)


// This is a compiler directive that includes libraries (For Visual Studio).
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")


//#pragma comment( linker, "/NODEFAULTLIB:libc.lib" ) 
//#pragma comment( linker, "/NODEFAULTLIB:libcmt.lib" ) 


// Memory leak check - Begin
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#include <iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
// Memory leak check - End


// Library includes
#include <stdio.h>
#include <string>

// Project includes
#include "Compiler.h"
#include <Common\Logger.h>

// Namespace declarations


int main(int argc, char *argv[])
{
	// Memory leak detection
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	printf("AtlasEngine ShaderCompiler v0.0\n");
	printf("\n");

	if ( argc <= 1 ) {
		printf("Usage:\t <program> <filename>.<ext>\n");
		printf("\n");

		return 1;
	}

	std::string filename = argv[1];

	Common::Logger *logger = new Common::Logger(0, "");

	Compiler compiler(logger);
	compiler.init();

	return compiler.process(filename);
}
