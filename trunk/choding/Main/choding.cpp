// choding.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "choding_stdafx.h"

#include "../Framework/ResourceManager/ResManager.h"

#include "../../chodingModule/Snowboard/Framework/Snowboard.h"

#include "../Framework/Profile/Profile.h"

LRESULT WINAPI MsgProc( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc( hWnd , msg , wParam , lParam );
}

int WINAPI WinMain( HINSTANCE hInst , HINSTANCE , LPSTR , INT )
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) , CS_CLASSDC , MsgProc , 0L , 0L ,
					GetModuleHandle(NULL) , NULL , NULL , NULL , NULL ,
					L"Choding" ,NULL };

	RegisterClassEx( &wc );

	HWND hWnd = CreateWindow( L"Choding" , L"Choding" , WS_OVERLAPPEDWINDOW , 100 , 100,
								500 , 500 ,
								GetDesktopWindow() , NULL ,
								wc.hInstance , NULL );

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
			//Do~~
		}
	}

	UnregisterClass( L"Choding" , wc.hInstance );
	return 0;
}