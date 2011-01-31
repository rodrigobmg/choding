#include "RendererDX9.h"

CRendererDX9::CRendererDX9()
{
	SetName( OBJECT_RENDERERDX9 );
}

CRendererDX9::~CRendererDX9()
{
	Destroy();
}

void CRendererDX9::UpdateFrame()
{
	if ( m_pd3dDevice == NULL )
		return;

	this->begin();

	this->render();

	this->end();
}

void CRendererDX9::render()
{
	
}

void CRendererDX9::begin()
{
	if ( m_pd3dDevice )
		m_pd3dDevice->Clear( 0 , NULL , D3DCLEAR_TARGET , D3DCOLOR_XRGB( 0, 0, 255 ),
		1.0f, 0 );


	if ( m_pd3dDevice )
		m_pd3dDevice->BeginScene();
}

void CRendererDX9::end()
{
	if (m_pd3dDevice)
	{
		m_pd3dDevice->EndScene();
	}

	if( m_pd3dDevice )
		m_pd3dDevice->Present( NULL , NULL , NULL , NULL );
}

HRESULT CRendererDX9::Create( HWND hWnd )
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

	return S_OK;
}

void CRendererDX9::Destroy()
{
	if ( m_pd3dDevice )
		m_pd3dDevice->Release();
	if ( m_pD3D )
		m_pD3D->Release();
}

void CRendererDX9::Clear()
{

}