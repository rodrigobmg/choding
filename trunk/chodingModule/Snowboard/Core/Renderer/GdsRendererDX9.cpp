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
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the D3DDevice
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	m_RootNode = GdsNodePtr( new GdsNode );

	return true;
}

void GdsRendererDX9::Update( float fAccumTime )
{
	m_pd3dDevice->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 128, 128, 128 ) , 1.f , 0 );
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		if ( m_RootNode )
			m_RootNode->Update( fAccumTime );

		m_pd3dDevice->EndScene();
	}

	m_pd3dDevice->Present( NULL , NULL , NULL , NULL );
}
