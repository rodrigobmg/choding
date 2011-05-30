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
 
 		RENDERER.Create( hWnd );
 		RESMGR.Create( RENDERER.GetDevice() );
 
 		UnregisterClass( L"Choding" , wc.hInstance );
	}

	virtual ~GdsResourceMgrTest()
	{

	}

	virtual void SetUp()
	{
		//»ý¼ºÀÚ
 		TCHAR respath[MAX_PATH];
 		_stprintf_s( respath , L"%s\\%s" , L"e:\\project\\choding\\application" , L"Resource" );
 		ASSERT_TRUE( RESMGR.CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( respath , L"ase;bmp;jpg;dds;tga" , true )) );
		GdsResTexturePtr ptex = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( L"banana.bmp" ) );
		LPDIRECT3DTEXTURE9 tex = ptex->Get();
	}

	virtual void TearDown()
	{
	}

	virtual void TestBody()
	{
	}

 	HWND hWnd;
};


TEST_F( GdsResourceMgrTest ,  Module )
{
	GdsRenderObjectPtr p;
	RESMGR.AllocRenderObject( p );
	RESMGR.FreeRenderObject( p );
}

#endif	