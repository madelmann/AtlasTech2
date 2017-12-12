
#pragma once
#pragma warning(disable : 4068)


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
#include <windows.h>
//#include <vld.h>

// Project includes
#include "ISCViewer.h"

// Namespace declarations


ISCViewer *app = 0;


LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( app ) {
		return app->handle( Common::Event(uMsg, wParam, lParam), hWnd );
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); 							// Return the default
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int /*nCmdShow*/)
{
	// Memory leak detection
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	app = new ISCViewer(hInstance);

	app->initialize(lpCmdLine);
	app->exec();

	delete app;

	return 0;
}
