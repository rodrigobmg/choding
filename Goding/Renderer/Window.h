#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

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
	Window(WNDPROC messageHandler, LPCTSTR title, int x, int y, int width, int height);

	//Destructor
	~Window();
};

#endif _RENDER_WINDOW_H_