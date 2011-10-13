#include "stdafx.h"

Jungding*	Jungding::ms_pAppWin = NULL;

Jungding::Jungding(HWND hwnd )
:m_hWnd(hwnd)
,m_pSnowBoard(NULL)
{
	Init();
}

Jungding::~Jungding()
{
	Destory();
}

LRESULT CALLBACK Jungding::MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

 	if ( ms_pAppWin && ms_pAppWin->m_pSnowBoard )
 	{
 		ms_pAppWin->m_pSnowBoard->MsgProc( hWnd, message, wParam, lParam );
 	}	

	return DefWindowProc( hWnd, message, wParam, lParam);

}

void Jungding::Init()
{
	if ( m_pSnowBoard == NULL )
	{
		m_pSnowBoard = new SnowBoard;		
		m_pSnowBoard->Init(m_hWnd);
	}
}

void Jungding::Destory()
{
 	if ( m_pSnowBoard != NULL )
 	{
 		delete m_pSnowBoard;
 		m_pSnowBoard = NULL;
 	}	
}

void Jungding::MsgIdle()
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

void Jungding::OnIdle()
{
	if ( m_pSnowBoard )
		m_pSnowBoard->OnIdle();
}