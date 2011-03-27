#ifndef _SUN_TESTAPP_WINMAIN_H_
#define _SUN_TESTAPP_WINMAIN_H_

#include "Precompiled.h"
#include "AppMain/TestApp.h"

//#include <crtdbg.h>
//#include <vld.h>



TestApp* g_pkTestApp = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdline, int iWinMode)
{
	static WCHAR lpszName[] = L"Test APP";
		
	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= TestApp::MsgProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( WHITE_BRUSH );
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= lpszName;
	RegisterClass(&wc);

	RECT  rect = {0, 0, 1024, 768};
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRect(&rect, dwStyle, false);
	HWND hWnd = CreateWindow(lpszName, lpszName, dwStyle, 0, 0,
				rect.right- rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
	
	
	ShowWindow(hWnd, iWinMode);
	UpdateWindow(hWnd);

	g_pkTestApp = new TestApp();
	assert(g_pkTestApp);

	g_pkTestApp->Start(hWnd);
	g_pkTestApp->End();

	Safe_Delete(g_pkTestApp);



	return 0;

}





