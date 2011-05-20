#include "Snowboard.h"

#include "../Core/GdsCoreFactory.h"
#include "../Core/Renderer/GdsRendererDX9.h"

#include "../System/Logger/logger.h"
#include "../System/PerformanceCheck/Performance.h"

#include "SceneNode/GdsNode.h"
#include "../System/Time/GdsSystemTime.h"
#include "../System/FrameMemory/GdsFrameMemory.h"
#include "Resource/Type/GdsResASE.h"
#include "InputSystem/GdsInputSystem.h"
#include "Terrain/GdsTerrain.h"

CSnowboard::CSnowboard()
:m_bMouseDrag(false)
{
	Clear(); 
}

CSnowboard::~CSnowboard()
{
	DestroyModule();
}

void CSnowboard::Clear()
{
	m_fFrameRate = 0.0f;
}

bool CSnowboard::InitModule( HWND hWnd )
{
	LOGGER.Init(NULL , NULL , NULL , NULL );
	FRAMEMEMORY.Init( 1024 * 1024 );
	LOG_WARNING_F( "Init FrameMemory = %d Byte" , FRAMEMEMORY.GetSize() );

	m_hWnd = hWnd;

	InitRenderer( hWnd );
	InitResource( RENDERER.GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	if ( RENDERER.Create( hWnd ) == false )
	{
		LOG_ERROR_F( "Fail to create renderer \n" );
		return false;
	}	

	GDS::SetMaxFrameRate( 0 );

	return false;
}

bool	CSnowboard::InitResource( LPDIRECT3DDEVICE9 device )
{
	//m_pResMgr = boost::shared_dynamic_cast< GdsResMgr >( GdsCoreFactory::CreateCore( CORE_RESOURCE ) );
	RESMGR.Create( device );
	return TRUE;
}

void CSnowboard::DestroyModule()
{
// 	if ( m_pRenderer )
// 		m_pRenderer->Release();
// 	if ( m_pResMgr )
// 		m_pResMgr->Release();	
}


void CSnowboard::OnIdle()
{
	if ( !GDS::MeasureTime() )
	{
		return;
	}

// 	if ( m_pRenderer )
// 	{		
// 		m_pRenderer->Update( GDS::GetAccumTime() );
// 		m_fFrameRate = 1.0f / GDS::GetFrameTime();
// 	}
	RENDERER.Update( GDS::GetAccumTime() );
	m_fFrameRate = 1.0f / GDS::GetFrameTime();
}


void CSnowboard::TestFunc()
{	
	LOG_WARNING_F( L"%s", L"logger init" );
	TCHAR curpath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, curpath );
	TCHAR respath[MAX_PATH];
	_stprintf_s( respath , L"%s\\%s" , curpath , L"Resource");

	RESMGR.CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( respath , L"ase;bmp;dds;tga;jpg" , true ) );

	RENDERER.GetRootNode()->SetName( L"ROOT" );

	GdsNodePtr mapNode = GdsNodePtr( new GdsNode );
	mapNode->SetName( L"height Map" );
	TERRAIN.MakeHeightMap( mapNode );
	RENDERER.GetRootNode()->AttachChild( mapNode );

}

HRESULT CSnowboard::MsgProc(  HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	INPUTSYSTEM.Record( hWnd , msg , wParam , lParam );
	
	if ( INPUTSYSTEM.GetKeyIsDown( VK_P ) )
		RENDERER.ToggleWireMode();

	if ( INPUTSYSTEM.GetKeyIsDown( VK_W ) )
		CAMMGR.MoveForwardEye( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_A ) )
		CAMMGR.MoveLeftEye( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_D ) )
		CAMMGR.MoveRightEye( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_S ) )
		CAMMGR.MoveBackEye( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_Q ) )
		CAMMGR.MoveUpEye( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_E ) )
		CAMMGR.MoveDownEye( 2.0f );

	
 	if ( m_bMouseDrag )
 	{
 		int x ,y ,z;
 		INPUTSYSTEM.GetMousePosDelta( x, y, z );
 		//LOG_WARNING_F( "Mouse DeltaPos %d %d %d\n" , x ,y,z );
 		//CAMMGR.MoveLeftLookat( 2.0f );
		if ( x > 0 )
			CAMMGR.MoveRightLookat( 0.05 );
		else
			CAMMGR.MoveLeftLookat( 0.05 );

		if ( y > 0 )
			CAMMGR.MoveDownLookat( 0.05 );
		else
			CAMMGR.MoveUpLookat( 0.05 );

		if ( z > 0 )
			CAMMGR.MoveForwardLookat( 0.05 );
		else
			CAMMGR.MoveBackLookat( 0.05 );
 	}
 
 	if ( INPUTSYSTEM.GetMouseIsDown( VM_LBTN ) )
 	{
 		m_bMouseDrag = true;
 	}	
 
 	if ( INPUTSYSTEM.GetMouseIsUp( VM_LBTN ) )
 	{
 		m_bMouseDrag = false;
 	}
	
 	if ( INPUTSYSTEM.GetKeyIsDown( VK_I ) )
 		CAMMGR.MoveForwardLookat( 2.0f );
 
 	if ( INPUTSYSTEM.GetKeyIsDown( VK_J ) )
 		CAMMGR.MoveLeftLookat( 2.0f );
 
 	if ( INPUTSYSTEM.GetKeyIsDown( VK_L ) )
 		CAMMGR.MoveRightLookat( 2.0f );
 
 	if ( INPUTSYSTEM.GetKeyIsDown( VK_K ) )
 		CAMMGR.MoveBackLookat( 2.0f );
 
 	if ( INPUTSYSTEM.GetKeyIsDown( VK_U ) )
 		CAMMGR.MoveUpLookat( 2.0f );
 
 	if ( INPUTSYSTEM.GetKeyIsDown( VK_O ) )
 		CAMMGR.MoveDownLookat( 2.0f );

	return true;
}

