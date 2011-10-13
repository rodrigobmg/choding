#include "Precompiled.h"
#include "Triangle.h"



ImplementRTTI(Triangle, sunRenderNode);


Triangle::Triangle():
m_pVertData(NULL)
{

}

Triangle::~Triangle()
{

}

void Triangle::Destory()
{
	Safe_Delete(m_pVertData);
}


HRESULT Triangle::CreateTriangle()
{
	assert(NULL != sunGraphicCore::GetCore());

	sunRenderer* pRenderer = sunGraphicCore::GetCore()->GetRenderer();
	assert(pRenderer);

	if( pRenderer->GetRendererID() != RENDERER_DX9)
	{
		assert(0&& "사용할 수 없는 환경");
		return S_FALSE;
	}

	sunRendererDX9* pRenderDX9 = static_cast<sunRendererDX9*>(pRenderer);
	assert(pRenderer);
	LPDIRECT3DDEVICE9 pD3DDevice = pRenderDX9->GetDevice();

	m_pVertData = new sunVertexData;
	assert(m_pVertData);

	m_pVertData->m_iVertexCount = 3;
	m_pVertData->m_iVertexSize  = sizeof(stTriangleVtx);


	stTriangleVtx* pTriangle = NULL;
	HRESULT hr;

	if( FAILED( hr = pD3DDevice->CreateVertexBuffer( 3 * sizeof(stTriangleVtx), 0, 0, 
		D3DPOOL_MANAGED, &m_pVertData->m_pVB, NULL) ) )
	{
		assert(0&& "Triangle::Create - Failed to CreateVertexBuffer!!");
		return S_FALSE;
	}

	if( FAILED( hr = m_pVertData->m_pVB->Lock(0, 3 * sizeof(stTriangleVtx), (void**)&pTriangle, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD) ) )  
	{
		assert(0&& "Triangle::Create - Failed to Lock Vertex Buffer!!");
		return S_FALSE;
	}

	pTriangle[0] = stTriangleVtx(-1.0f,-1.0f, 0.0f, 0xffff0000);
	pTriangle[1] = stTriangleVtx( 1.0f,-1.0f, 0.0f, 0xff00ff00);
	pTriangle[2] = stTriangleVtx( 0.0f, 1.0f, 0.0f, 0xff00ffff);

	m_pVertData->m_pVB->Unlock();

	D3DVERTEXELEMENT9 pVertexElements[] = 
	{ 
		{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,	0 }, 
		{ 0,	12,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_COLOR,		0 }, 
		D3DDECL_END()
	}; 


	if( FAILED( hr = pD3DDevice->CreateVertexDeclaration( pVertexElements, &m_pVertData->m_pVertexDeclaration ) ) ) 
	{
		assert(0&& "Triangle::Create - Failed to CreateVertexDeclaration" );	
		return S_FALSE;
	}

	return S_OK;
}


void Triangle::GetRenderOperation(sunRenderOperation& op)
{
	op.m_bUseIndexes = false;
	op.m_PrimType	 = D3DPT_TRIANGLELIST;
	op.m_pVertexData = m_pVertData;
	op.m_dwPrimCount = m_pVertData->m_iVertexCount / 2;
}