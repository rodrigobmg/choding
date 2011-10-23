#pragma once

//-------------------------------------------------
// Licence
//-------------------------------------------------
//Copyright (c) 2010 Celal Cansin Kayi
//
//This software is provided 'as-is', without any express or implied
//warranty. In no event will the authors be held liable for any damages
//arising from the use of this software.
//
//Permission is granted to anyone to use this software for any purpose,
//including commercial applications, and to alter it and redistribute it
//freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//-------------------------------------------------
//
//-------------------------------------------------

#include <windows.h>
#include <cstdlib>

class Window
{
protected:
	WNDCLASS windowClass;
	HWND window;

public:

	HWND getHandle() { return window; }
	HINSTANCE getInstance() { return windowClass.hInstance; }
	void Show() { ShowWindow(window, SW_SHOWDEFAULT); }
	void setSize(int width, int height) { SetWindowPos(window, HWND_NOTOPMOST, NULL, NULL, width, height, SWP_NOACTIVATE | SWP_NOMOVE); }

	//Constructor
	Window(WNDPROC messageHandler, LPCTSTR title, int x, int y, int width, int height)
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

	//Destructor
	~Window()
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
};
