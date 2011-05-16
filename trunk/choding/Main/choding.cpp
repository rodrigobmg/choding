// choding.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "choding_stdafx.h"

#include "../../chodingModule/Snowboard/Framework/Snowboard.h"

CSnowboard* g_pSnowboard = NULL;

bool InitModule( HWND hWnd )
{
	//모듈 할당및 초기화
	g_pSnowboard = new CSnowboard;
	g_pSnowboard->DestroyModule();

	if ( !g_pSnowboard->InitModule( hWnd ) )
		return false;

	return true;
}

void DestroyModule()
{
	SAFE_DELETE(g_pSnowboard);
}

LRESULT WINAPI MsgProc( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		g_pSnowboard->MsgProc( hWnd , msg , wParam , lParam );
	}

	return DefWindowProc( hWnd , msg , wParam , lParam );
}

bool OnIdle()
{
	if ( g_pSnowboard )
	{
		g_pSnowboard->OnIdle();
		float rate = g_pSnowboard->GetFrameRate();
		
		char buffer[256] = {0,};
		sprintf_s( buffer , 256 , "FrameRate = %d" , (int)rate);
		::SetWindowTextA( g_pSnowboard->GetWnd() , buffer );
		
	}

	return true;
}

int WINAPI WinMain( HINSTANCE hInst , HINSTANCE , LPSTR , INT )
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) , CS_CLASSDC , MsgProc , 0L , 0L ,
					GetModuleHandle(NULL) , NULL , NULL , NULL , NULL ,
					L"Choding" ,NULL };

	RegisterClassEx( &wc );

	HWND hWnd = CreateWindow( L"Choding" , L"Choding" , WS_OVERLAPPEDWINDOW , 100 , 100,
								800 , 600,
								GetDesktopWindow() , NULL ,
								wc.hInstance , NULL );

	if ( !InitModule( hWnd ) )
	{
		UnregisterClass( L"Choding" , wc.hInstance );
		DestroyModule();	
		return 0;
	}

	ShowWindow( hWnd , SW_SHOWDEFAULT );
	UpdateWindow( hWnd );
	MSG msg;
	ZeroMemory( &msg , sizeof( msg ) );
	while( msg.message != WM_QUIT )
	{
		if( PeekMessageW( &msg , NULL , 0U , 0U , PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessageW( &msg );
		}
		else
		{
			if ( !OnIdle() )
				break;
		}
	}

	UnregisterClass( L"Choding" , wc.hInstance );
	DestroyModule();
	return 0;
}