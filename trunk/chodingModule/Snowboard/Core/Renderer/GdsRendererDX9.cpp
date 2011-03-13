#include "GdsRendererDX9.h"


// 사용자정의 정점 버텍스
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)


GdsRendererDX9::GdsRendererDX9()
{
	SetName( OBJECT_RENDERERDX9 );
	m_pd3dDevice	= NULL;
	m_pD3D			= NULL;
}

GdsRendererDX9::~GdsRendererDX9()
{
	vRelease();
}

HRESULT GdsRendererDX9::vRelease()
{
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pd3dDevice);
	m_RootNode->RemoveAllChild();
	m_CamManager->DetachAll();
	return true;
}

void GdsRendererDX9::vClear()
{

}

HRESULT GdsRendererDX9::vCreate( HWND hWnd )
{
	// Create the D3D object.
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

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
		return E_FAIL;
	}

	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	//m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	//m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	// Turn off D3D lighting
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	setRootNodeAndCamNode();

	return true;
}

void GdsRendererDX9::Update( float fAccumTime )
{
	m_pd3dDevice->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 128, 128, 128 ) , 1.f , 0 );
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		if ( m_RootNode )
			m_RootNode->Update( fAccumTime );

// 		D3DMATERIAL9 mtrl;
// 		ZeroMemory( &mtrl , sizeof( D3DMATERIAL9 ) );
// 		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
// 		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
// 		mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
// 		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
// 		m_pd3dDevice->SetMaterial( &mtrl );
// 
// 		D3DXVECTOR3 vecDir;
// 		D3DLIGHT9	light;
// 		ZeroMemory( &light , sizeof( D3DLIGHT9 ) );
// 		light.Type = D3DLIGHT_DIRECTIONAL;
// 		light.Diffuse.r = 1.0f;
// 		light.Diffuse.g = 1.0f;
// 		light.Diffuse.b = 1.0f;
// 		vecDir = D3DXVECTOR3( cosf( timeGetTime() / 350.f ) , 1.0f , sinf( timeGetTime()/350.f )) ;
// 		D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction , &vecDir );
// 		light.Range = 1000.0f;
// 		m_pd3dDevice->SetLight( 0 , &light );
// 		m_pd3dDevice->LightEnable( 0 , TRUE );
// 		m_pd3dDevice->SetRenderState( D3DRS_LIGHTING , TRUE );
// 		m_pd3dDevice->SetRenderState( D3DRS_AMBIENT , 0x00202020 );

		m_pd3dDevice->EndScene();
	}

	m_pd3dDevice->Present( NULL , NULL , NULL , NULL );
}

void GdsRendererDX9::setRootNodeAndCamNode()
{
	m_RootNode = GdsNodePtr( new GdsNode );
	m_RootNode->SetDevice( m_pd3dDevice );

	m_CamManager = GdsCameraManagerDX9Ptr( new GdsCameraManagerDX9 );
	m_CamManager->Create();

	GdsCameraNodePtr	camnode = GdsCameraNodePtr( new GdsCameraNode );

	D3DXVECTOR3 vEyePt( 0.0f, 0.0f,-30.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );

	camnode->SetLootAtLH( vEyePt , vLookatPt , vUpVec );
	camnode->SetPerspective( D3DX_PI/4, 1.0f, 1.0f, 1000.0f );
	m_CamManager->Attach( camnode );

	GdsCameraNodePtr camera = m_CamManager->GetCamNode( 0 );
	m_RootNode->AttachChild( camera );
}
