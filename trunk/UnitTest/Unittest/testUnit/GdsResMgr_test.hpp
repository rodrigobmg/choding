#ifndef _UNITTEST_GDSRESMGR_H_
#define _UNITTEST_GDSRESMGR_H_

#include "..\..\..\chodingModule\Snowboard\Core\Resource\GdsResMgr.h"
#include "..\..\..\chodingModule\Snowboard\Core\Renderer\GdsRendererDX9.h"

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

class GdsResourceMgrTest : public ::testing::Test
{
public:
	GdsResourceMgrTest()
	{
		WNDCLASSEX wc = { sizeof(WNDCLASSEX) , CS_CLASSDC , MsgProc , 0L , 0L ,
			GetModuleHandle(NULL) , NULL , NULL , NULL , NULL ,
			L"Choding" ,NULL };

		RegisterClassEx( &wc );

		hWnd = CreateWindow( L"Choding" , L"Choding" , WS_OVERLAPPEDWINDOW , 100 , 100,
			300 , 300 ,
			GetDesktopWindow() , NULL ,
			wc.hInstance , NULL );
		
		UpdateWindow( hWnd );

		resmgr = GdsResMgrPtr( new GdsResMgr );
		renderer = GdsRendererDX9Ptr( new GdsRendererDX9 );
		renderer->Create( hWnd );
		resmgr->Create( renderer->GetDevice() );
	}

	virtual ~GdsResourceMgrTest()
	{
//		UnregisterClass( L"Choding" , wc.hInstance );
	}

	virtual void SetUp()
	{
		//持失切
		TCHAR curpath[ MAX_PATH ];
		GetCurrentDirectory( MAX_PATH, curpath );
		TCHAR respath[MAX_PATH];
		_stprintf_s( respath , L"%s\\%s" , L"E:\\Project\\choding\\Application" , L"Resource" );
		resmgr->CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( L"md2", respath , L"md2" , true ) );
		resmgr->LoadRes( L"md2" );
	}

	virtual void TearDown()
	{
		//社瑚切
		resmgr->ReleaseRes( L"md2" );
		resmgr->ReleaseList( L"md2" );
	}

	virtual void TestBody()
	{
	}

	//WNDCLASSEX wc;
	HWND hWnd;
	GdsResMgrPtr resmgr;
	GdsRendererDX9Ptr	renderer;
};


TEST_F( GdsResourceMgrTest ,  Module )
{
	GdsResourceMgrTest test;
}

#endif	