#include "GdsRendererDX9.h"
#include "../Camera/GdsCameraManagerDX9.h"
#include "InputSystem/GdsInputSystem.h"
#include "../../System/Logger/logger.h"

GdsRendererDX9::GdsRendererDX9() :
m_bWireMode( false ) ,
m_pd3dDevice( NULL ) ,
m_pD3D( NULL )
{
	SetName( OBJECT_RENDERERDX9 );
}

GdsRendererDX9::~GdsRendererDX9()
{
	vRelease();
}

HRESULT GdsRendererDX9::vRelease()
{
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pd3dDevice);
	if ( m_RootNode )
		m_RootNode->RemoveAllChild();
	return true;
}

void GdsRendererDX9::vClear()
{

}

bool GdsRendererDX9::vCreate( HWND hWnd )
{
	// Create the D3D object.
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		LOG_ERROR_F( "Fail to create d3d\n");
		return false;
	}

	// Set up the structure used to create the D3DDevice. Since we are now
	// using more complex geometry, we will create a device with a zbuffer.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Create the D3DDevice
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pd3dDevice ) ) )
	{
		LOG_ERROR_F("Fail to create device\n");
		return false;
	}

	setRootNodeAndCamNode();
	
	m_RenderFrameList = GdsRenderFramePtr( new GdsRenderFrame );

	return true;
}

void GdsRendererDX9::vUpdate( float fAccumTime )
{	
	if ( m_bWireMode )
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	else
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	m_pd3dDevice->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 128, 128, 128 ) , 1.f , 0 );

	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{	
		if ( CAMMGR.GetCurCam() )
		{
			CAMMGR.Update( fAccumTime );
			m_pd3dDevice->SetTransform( D3DTS_VIEW , &(CAMMGR.GetCurCam()->GetViewMat()));
			m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &(CAMMGR.GetCurCam()->GetProjMat()) );
		}

		if ( m_RootNode )
			m_RootNode->Update( fAccumTime );					

		m_RenderFrameList->Render( m_pd3dDevice );
 		m_pd3dDevice->EndScene();
	}

	m_pd3dDevice->Present( NULL , NULL , NULL , NULL );
}

void GdsRendererDX9::setRootNodeAndCamNode()
{
	m_RootNode = GdsNodePtr( new GdsNode );
		
	GdsCameraPtr	camnode = GdsCameraPtr( new GdsCamera );

	D3DXVECTOR3 vEyePt( 1.0f, 100.0f,-100.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );

	camnode->SetLootAtLH( vEyePt , vLookatPt , vUpVec );

	GdsFrustum frustum( -0.5 , 0.5 , -0.5 , 0.5 , 1.f , 1000.f , false );
	camnode->SetFrustum( frustum );

 	CAMMGR.Attach( camnode );
  	CAMMGR.SetCurCam( 0 );
}
