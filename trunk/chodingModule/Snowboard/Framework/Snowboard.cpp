#include "Snowboard.h"

#include "../Core/GdsCoreFactory.h"
#include "../Core/Renderer/GdsRendererDX9.h"
#include "../Core/Resource/Type/GdsResTexture.h"

#include "../Utility/Log/logger.h"
#include "../Utility/PerformanceCheck/Performance.h"

#include "../System/Thread/GdsThreadPool.h"
#include "SceneNode/GdsNode.h"
#include "Resource/Type/GdsResMD2.h"
#include "SceneNode/GdsBillboardNode.h"
#include "../System/Time/GdsSystemTime.h"

CSnowboard::CSnowboard()
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

	InitRenderer( hWnd );
	InitResource( m_pRenderer->GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	m_pRenderer = boost::shared_dynamic_cast< GdsRendererDX9 >( GdsCoreFactory::CreateCore( CORE_RENDERER ) );
	m_pRenderer->Create( hWnd );
	m_pRenderer->GetRootNode()->SetDevice( m_pRenderer->GetDevice() );
	
	return TRUE;
}

bool	CSnowboard::InitResource( LPDIRECT3DDEVICE9 device )
{
	m_pResMgr = boost::shared_dynamic_cast< GdsResMgr >( GdsCoreFactory::CreateCore( CORE_RESOURCE ) );
	m_pResMgr->Create( device );
	return TRUE;
}

void CSnowboard::DestroyModule()
{
	if ( m_pRenderer )
		m_pRenderer->Release();
	if ( m_pResMgr )
		m_pResMgr->Release();	
}


void CSnowboard::OnIdle()
{
	if ( !GDS::MeasureTime() )
	{
		return;
	}

	if ( m_pRenderer )
	{		
		m_pRenderer->Update( GDS::GetAccumTime() );
		m_fFrameRate = 1.0f / GDS::GetFrameTime();
	}
}


void CSnowboard::TestFunc()
{	
	LOG_WARNING_F( L"%s", L"logger init" );
	TCHAR curpath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, curpath );
	TCHAR respath[MAX_PATH];
	_stprintf_s( respath , L"%s\\%s" , curpath , L"Resource");

	if ( m_pResMgr )
	{	 
 		m_pResMgr->CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( L"md2" , respath , L"md2" , true ) );
		m_pResMgr->LoadRes( L"md2" );

		m_pResMgr->CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( L"tex" , respath , L"bmp;dds;tga" , true ) );
		m_pResMgr->LoadRes( L"tex" );
	}

	GdsResMD2Ptr resmesh = boost::shared_dynamic_cast< GdsResMD2 >( m_pResMgr->Get( L"md2" , L"tree2.md2" ) );
	GdsResMD2Ptr res_tree1 = boost::shared_dynamic_cast< GdsResMD2 >( m_pResMgr->Get( L"md2" , L"fruit.md2" ) );

	GdsPropertyStatePtr pProperty = GdsPropertyStatePtr( new GdsPropertyState );
	if( resmesh )
	{
		GdsNodePtr mesh = GdsNodePtr( new GdsNode );		
		
		pProperty->GetPolygonProperty()->SetVB(resmesh->GetVertexBufferPtr() );
		pProperty->GetPolygonProperty()->SetFVF( resmesh->GetFVF() );
		pProperty->GetPolygonProperty()->SetPrimitive( resmesh->GetPrimitive() );
		pProperty->GetPolygonProperty()->SetVertexFormatSize( resmesh->GetVertexSize() );
		pProperty->GetTextureProperty()->SetTexture( resmesh->GetTexturePtr() );
		mesh->SetPropertyState( pProperty );
		mesh->SetScale( 0.5 );
		mesh->SetDrawAxis( true );
		m_pRenderer->GetRootNode()->AttachChild( mesh );

		GdsNodePtr mesh1 = GdsNodePtr( new GdsNode );

		GdsPropertyStatePtr pProperty_tree = GdsPropertyStatePtr( new GdsPropertyState );
		pProperty_tree->GetPolygonProperty()->SetVB( res_tree1->GetVertexBufferPtr() );
		pProperty_tree->GetPolygonProperty()->SetFVF( res_tree1->GetFVF() );
		pProperty_tree->GetPolygonProperty()->SetPrimitive( res_tree1->GetPrimitive() );
		pProperty_tree->GetPolygonProperty()->SetVertexFormatSize( res_tree1->GetVertexSize() );
		pProperty_tree->GetTextureProperty()->SetTexture( res_tree1->GetTexturePtr() );

		mesh1->SetPropertyState( pProperty_tree );
		mesh1->SetTranslate( -10 , 0 , 0 );
		
		//mesh->AttachChild( mesh1 );
		//mesh1->SetCullType( GdsNode::CULL_ON );

	}	

	//create billboard
	GdsResTexturePtr restex = boost::shared_dynamic_cast< GdsResTexture >( m_pResMgr->Get( L"tex" , L"banana.bmp" ) );
	if ( restex )
	{
		GdsBillboardNodePtr billboard = GdsBillboardNodePtr( new GdsBillboardNode );
		GdsPropertyStatePtr billboardproperty = GdsPropertyStatePtr( new GdsPropertyState );
		billboardproperty->GetTextureProperty()->SetTexture( restex->GetTexturePtr() );
		billboard->SetPropertyState( pProperty );
		billboard->SetTranslate( 0 , 10 , 10 );
		m_pRenderer->GetRootNode()->AttachChild( billboard );
		//billboard->SetCullType( GdsNode::CULL_ON );
	}

}

HRESULT CSnowboard::MsgProc()
{
	return true;
}