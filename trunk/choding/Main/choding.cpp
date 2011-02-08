// choding.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "choding_stdafx.h"

#include "../Framework/ResourceManager/ResManager.h"

#include "../Framework/Profile/Profile.h"
#include "../../chodingModule/Snowboard/Framework/Snowboard.h"

CSnowboard* g_pSnowboard = NULL;

bool InitModule( HWND hWnd )
{
	//��� �Ҵ�� �ʱ�ȭ
	g_pSnowboard = new CSnowboard;
	g_pSnowboard->DestroyModule();

	if ( !g_pSnowboard->InitModule( hWnd ) )
		return false;

	return true;
}

void DestroyModule()
{
	if ( g_pSnowboard )
		g_pSnowboard->DestroyModule();
}

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

bool Update()
{
	if ( g_pSnowboard )
		g_pSnowboard->Update();

	return true;
}

int WINAPI WinMain( HINSTANCE hInst , HINSTANCE , LPSTR , INT )
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) , CS_CLASSDC , MsgProc , 0L , 0L ,
					GetModuleHandle(NULL) , NULL , NULL , NULL , NULL ,
					L"Choding" ,NULL };

	RegisterClassEx( &wc );

	HWND hWnd = CreateWindow( L"Choding" , L"Choding" , WS_OVERLAPPEDWINDOW , 100 , 100,
								300 , 300 ,
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
			if ( !Update() )
				break;
		}
	}

	UnregisterClass( L"Choding" , wc.hInstance );
	DestroyModule();
	return 0;
}