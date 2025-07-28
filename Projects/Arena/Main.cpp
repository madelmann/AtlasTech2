
// Library includes
#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>

// Project includes
#include "Arena.h"

// Namespace declarations

Arena *app = nullptr;

int main(int argc, char* argv[])
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
		return -1;
	}

	// Create application instance
	app = new Arena();

	// Build command line string from argc/argv
	std::string cmdLine;
	for (int i = 1; i < argc; ++i) {
		if (i > 1) cmdLine += " ";
		cmdLine += argv[i];
	}

	// Initialize and run application
	app->initialize(cmdLine.c_str());
	app->exec();

	// Cleanup
	delete app;
	app = nullptr;

	SDL_Quit();
	return 0;
}
