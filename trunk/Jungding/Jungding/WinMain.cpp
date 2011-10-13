#include "stdafx.h"

#ifndef _JUNGDING_MAIN_H_
#define _JUNGDING_MAIN_H_

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdline, int iWinMode)
{
	static WCHAR lpszName[] = L"Jungding";
		
	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= Jungding::MsgProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( WHITE_BRUSH );
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= lpszName;
	RegisterClass(&wc);

	RECT  rect = {0, 0, 640, 480};
	DWORD dwStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rect, dwStyle, false);
	HWND hWnd = CreateWindow(lpszName, lpszName, dwStyle, 0, 0,
				rect.right- rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
	
	ShowWindow(hWnd, iWinMode);
	UpdateWindow(hWnd);

	Jungding::ms_pAppWin = new Jungding(hWnd);
	Jungding::ms_pAppWin->MsgIdle();

	delete Jungding::ms_pAppWin;
	return 0;
}

#endif