#include "StdAfx.h"
#include "Window.h"

Window::Window( WNDPROC messageHandler, LPCTSTR title, int x, int y, int width, int height )
{
	//Uint style - The style of the window
	windowClass.style = CS_DBLCLKS;
	//windowClass.style = CS_DBLCLKS | CS_NOCLOSE;

	//WNDPROC lpfnWndProc - The message handler to use
	windowClass.lpfnWndProc = messageHandler;

	// int cbClsExtra & cbWndExtra - Extra bytes to assign.
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;

	//HINSTANCE hInstance - The thread where this window will be created
	windowClass.hInstance = GetModuleHandle(NULL);

	//HICON hIcon - The big icon for this window class
	windowClass.hIcon = NULL;

	//HCURSOR hCursor - The mouse cursor to show in this window
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	ShowCursor(false);

	//HRBRUSH hbrBackground - The background of the window +2 is black
	windowClass.hbrBackground = NULL; //DirectX will specify the colour

	//LPCTSTR lpszMenuName - The menu resource to use
	windowClass.lpszMenuName = NULL;

	//LPCTSTR lpszClassName - What name to define this new window class
	windowClass.lpszClassName = title;

	RegisterClass(&windowClass);

	window = CreateWindowW(windowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW, x, y, width, height, GetDesktopWindow(), NULL, windowClass.hInstance, NULL);

	if(window == NULL) throw(TEXT("Window Constructor: Failed to create a new window!"));
}

Window::~Window()
{
	//Un-Register the window class using the same parameters as was used to create it
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	//Destroy the window as well, just to be safe
	if(window != NULL) 
	{
		DestroyWindow(window);
		window = NULL;
	}
}