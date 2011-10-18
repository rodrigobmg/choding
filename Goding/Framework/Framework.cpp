#include "stdafx.h"
#define DLLEXPORT
#include "Framework.h"


Framework::Framework(HWND hwnd )
:m_hWnd(hwnd)
{
	Init();
}

Framework::~Framework()
{
	Destory();
}

void Framework::Init()
{
}

void Framework::Destory()
{ 
}

void Framework::MsgIdle()
{
	while(true)
	{
		MSG msg;
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if(msg.message == WM_QUIT)
				break;

			if( !TranslateAccelerator( m_hWnd, NULL, &msg ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			OnIdle();
		}
	}
}

void Framework::OnIdle()
{
}