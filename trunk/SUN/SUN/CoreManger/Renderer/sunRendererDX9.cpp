#include "Precompiled.h"
#include "sunRendererDX9.h"


sunRendererDX9::sunRendererDX9()
: m_pD3DDevice(NULL),
  m_pD3D(NULL),
  m_pFont(NULL),
  m_pD3DDefaultSatate(NULL),
  m_spProgressCamera(NULL)
  
{
	ZeroMemory(&m_pD3DCaps, sizeof(D3DCAPS9));
	ZeroMemory(&m_D3DBackDesc, sizeof(D3DSURFACE_DESC));

	m_eRendererID = RENDERER_DX9;
}

sunRendererDX9::~sunRendererDX9()
{

}

void sunRendererDX9::DestroyRenderer()
{	
	Safe_Release(m_pFont);
	Safe_Release(m_pD3DDefaultSatate);
	Safe_Release(m_pD3DDevice);
	Safe_Release(m_pD3D);	

	m_spProgressCamera = NULL;

}

HRESULT sunRendererDX9::InitializeRenderer()
{
	assert(NULL == m_pD3DDevice);
	HRESULT hr;


	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	assert(m_pD3D);

	m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_pD3DCaps);

	
	if(m_pD3DCaps.VertexShaderVersion < D3DVS_VERSION(2,0) || m_pD3DCaps.PixelShaderVersion < D3DPS_VERSION(2,0) )
	{
		assert(NULL&&"VertexShaderVersion < 2.0");
		return E_FAIL;
	}

	// ����̽� ���� ����ü
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp));
	d3dpp.Windowed					= true;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

	//����̽� ����
	if( FAILED(hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_pCoreManager->GetWinHWND(),
		D3DCREATE_MULTITHREADED | D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice) ))
	{
		assert(NULL&&"m_pD3DDevice DirectX Device Create Fail!!!");
		return hr;
	}


	// ����� ������ ȭ�鿡 ����� ��Ʈ ����
	D3DXCreateFont( m_pD3DDevice, 12, 0, 0, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"����ü", &m_pFont);

	// �⺻ ������Ʈ ����
	if( FAILED( hr = SetDefaultState()))
		return hr;

	
	return hr;
}

bool sunRendererDX9::BeginRender()
{
	assert(m_pD3DDevice);

	m_iPolygonCount = 0;
	
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, m_bIsWireFrame? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	m_pD3DDevice->BeginScene();

	return true;
}

void sunRendererDX9::RenderObject( const vector<sunRenderNodePtr>& RenderList )
{
	
	sunRenderOperation RenderOp;

	BOOST_FOREACH( sunRenderNodePtr spObject, RenderList )
	{
		spObject->GetRenderOperation( RenderOp );

		assert( RenderOp.m_pVertexData->m_iVertexCount > 0 );
		if(RenderOp.m_pVertexData->m_iVertexCount < 0 )
		{
			return;
		}

		m_pD3DDevice->SetVertexDeclaration( RenderOp.m_pVertexData->m_pVertexDeclaration );
		m_pD3DDevice->SetStreamSource(0, RenderOp.m_pVertexData->m_pVB, 0, RenderOp.m_pVertexData->m_iVertexSize);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &spObject->GetWorldMatrix() );
		assert( RenderOp.m_dwPrimCount > 0);
			
		HRESULT hr;
		if( RenderOp.m_bUseIndexes )	
		{
			if( FAILED( hr = m_pD3DDevice->SetIndices( RenderOp.m_pIndexData->m_pIB ) ) )
			{
				assert(0 && "SetIndices is Failed" );
			}
			
			if( FAILED( hr = m_pD3DDevice->DrawIndexedPrimitive(
				RenderOp.m_PrimType, RenderOp.m_pVertexData->m_iVertexStart, 0,
				RenderOp.m_pVertexData->m_iVertexCount, RenderOp.m_pIndexData->m_iIndexStart, RenderOp.m_dwPrimCount ) ) )
			{
				assert(0 && "DrawIndexedPrimitive is FAILED" );
			}
		}

		else
		{
			if( FAILED( hr = m_pD3DDevice->DrawPrimitive( RenderOp.m_PrimType, RenderOp.m_pVertexData->m_iVertexStart, RenderOp.m_dwPrimCount ) ) )
			{
				assert(0 && "DrawPrimitive is Failed");
			}

		}


	}


}




bool sunRendererDX9::EndRender()
{
	assert(m_pD3DDevice);

	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	//�⺻ ���� ������Ʈ�� ����
	m_pD3DDefaultSatate->Apply();

	return true;
}

HRESULT sunRendererDX9::SetDefaultState()
{
	assert(m_pD3DDevice);
	
	HRESULT hr;
	//����� ���� ������
	LPDIRECT3DSURFACE9 pSurface;
	m_pD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface );
	ZeroMemory( &m_D3DBackDesc, sizeof( D3DSURFACE_DESC ) );

	assert(pSurface);
	pSurface->GetDesc( &m_D3DBackDesc );
	pSurface->Release();

	// �⺻ ������Ʈ ����
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE );		// �ø� ���� CCW
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,	TRUE );				// Z ���۱���� �Ҵ�.
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING,	FALSE );			// ��������� ����.
	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,	D3DFILL_SOLID );
	//g_pD3DDevice->SetRenderState( D3DRS_CLIPPING,	TRUE );

	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	// ��������� State�� ����
	if( FAILED( hr = m_pD3DDevice->CreateStateBlock( D3DSBT_ALL, &m_pD3DDefaultSatate ) ) )
	{
		assert(NULL &&" SetDefaultSatate Create Fail");
		return hr;
	}

	return hr;

}




void sunRendererDX9::SetCameraData( sunCameraPtr spCamera )
{
	assert( spCamera );
	assert( m_pD3DDevice);

	m_spProgressCamera = spCamera;
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_spProgressCamera->GetViewMatrix() );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_spProgressCamera->GetProjectionMatrix() );
	

}