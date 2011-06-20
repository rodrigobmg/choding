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
#include "../System/Thread/GdsThreadPool.h"

CSnowboard::CSnowboard()
:m_iRenderobjectCount(0)
,m_fFrameRate(0.0f)
,m_pRootNode( (GdsNode*)NULL )
{
	Clear(); 
}

CSnowboard::~CSnowboard()
{
	DestroyModule();
}

void CSnowboard::Clear()
{
	if ( m_pRootNode )
		m_pRootNode->RemoveAllChild();
}

bool CSnowboard::InitModule( HWND hWnd )
{
	LOGGER.Init(NULL , NULL , NULL , NULL );
	FRAMEMEMORY.Init( 1024 * 1024 );
	LOG_WARNING_F( "Init FrameMemory = %d Byte" , FRAMEMEMORY.GetSize() );
	
	int thread_count = THREADPOOL.GetTotalCountofThread();
	LOG_CYAN_F( "Make Thread = %d cont" , thread_count );

	m_hWnd = hWnd;

	InitRenderer( hWnd );
	InitResource( RENDERER.GetDevice() );
	InitRootNode();
	InitCamMgr();

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

	GDS::SetMaxFrameRate( 500.f );

	return false;
}

bool	CSnowboard::InitResource( LPDIRECT3DDEVICE9 device )
{
	RESMGR.Create( device );
	return TRUE;
}

bool CSnowboard::InitCamMgr()
{
	CAMMGR.Init();

	return CAMMGR.GetCurCam() != NULL ? true : false;
}

void CSnowboard::DestroyModule()
{
}


void CSnowboard::OnIdle()
{
	if ( !GDS::MeasureTime() )
	{
		return;
	}

	Update( GDS::GetAccumTime() );
	Render();	
}

void CSnowboard::Update(float fAccumTime)
{
	CAMMGR.Update( fAccumTime );
	TERRAIN.Update( fAccumTime );

	if ( m_pRootNode )
		m_pRootNode->Update( fAccumTime );					
}

void CSnowboard::Render()
{
	if ( RENDERER.EnableRendering() == false )
		return;

	GdsBGThread* thread = THREADPOOL.GetThread( 0 );
	if ( thread == NULL )
		return;

	//void	Push( _OWNER* pthis , _PARAMETER para , _FP fp )
	//thread->Push( &RENDERER , 0.f , &GdsRendererBase::RenderFrame );
	RENDERER.RenderFrame( 0.f );	
	m_fFrameRate = 1.0f / GDS::GetFrameTime();
	m_iRenderobjectCount = RENDERER.GetRenderFrame()->GetRenderObjectCount();
}


void CSnowboard::TestFunc()
{	
	LOG_WARNING_F( L"%s", L"logger init" );
	TCHAR curpath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, curpath );
	TCHAR respath[MAX_PATH];
	_stprintf_s( respath , L"%s\\%s" , curpath , L"Resource");

	RESMGR.CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( respath , L"ase;bmp;dds;tga;jpg" , true ) );

	m_pRootNode->SetName( L"ROOT" );

	TERRAIN.MakeHeightMap();
}

HRESULT CSnowboard::MsgProc(  HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	INPUTSYSTEM.Record( hWnd , msg , wParam , lParam );
		
	return true;
}

bool CSnowboard::InitRootNode()
{
	if ( m_pRootNode == NULL )
	{
		m_pRootNode = GdsNodePtr( new GdsNode );
	}

	return m_pRootNode == NULL ? false : true ;
}
