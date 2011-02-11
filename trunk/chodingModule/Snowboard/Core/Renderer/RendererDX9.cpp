#include "RendererDX9.h"


// 사용자정의 정점 버텍스
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)


CRendererDX9::CRendererDX9()
{
	SetName( OBJECT_RENDERERDX9 );
	m_pVertexBuffer = NULL;
	m_pd3dDevice	= NULL;
	m_pD3D			= NULL;
}

CRendererDX9::~CRendererDX9()
{
	Release();
}

HRESULT CRendererDX9::Release()
{
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pVertexBuffer);

	return S_OK;
}

void CRendererDX9::Clear()
{

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

	initvb();

	return S_OK;
}

void CRendererDX9::UpdateFrame()
{
	//this->begin();

	this->render();

	//this->end();
}



void CRendererDX9::render()
{
	if ( m_pd3dDevice == NULL )
		return;

	m_pd3dDevice->Clear( 0 , NULL , D3DCLEAR_TARGET , D3DCOLOR_XRGB( 0, 0, 255 ),
		1.0f, 0 );

	if ( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		m_pd3dDevice->SetStreamSource( 0, m_pVertexBuffer , 0, sizeof( CUSTOMVERTEX ) );
		m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , 1 , 2 );

		m_pd3dDevice->EndScene();
	}

	m_pd3dDevice->Present( NULL , NULL , NULL , NULL );
}

void CRendererDX9::begin()
{	
}

void CRendererDX9::end()
{
}


HRESULT CRendererDX9::initvb()
{
	CUSTOMVERTEX vertices[] = 
	{
		{ 100.f, 10.0f, 0.f, 1.0f, 0xffff0000,	},
		{ 250.f, 250.f , 0.f, 1.0f, 0xff00ff00,},
		{ 50.f, 250.f , 0.f, 1.0f, 0xff00ffff, },
		{ 50.f , 100.f , 0.f , 1.0f , 0xf0f0f0ff, }, 
		{ 10.f , 50.f , 0.f , 1.0f , 0xfff000ff, },
		{ 300.f , 300.f , 0.f , 1.0f , 0x000f0000, },
	};

	if ( FAILED(m_pd3dDevice->CreateVertexBuffer( 6*sizeof(CUSTOMVERTEX),
		0,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&m_pVertexBuffer,
		NULL )) )
	{
		return E_FAIL;
	}

	VOID* pVertices;
	if( FAILED(m_pVertexBuffer->Lock( 0, sizeof(vertices),
		(void**)&pVertices, 0 )))
		return E_FAIL;

	memcpy( pVertices , vertices , sizeof(vertices ) );
	m_pVertexBuffer->Unlock();

	return S_OK;
}