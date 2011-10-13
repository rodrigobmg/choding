#include "Precompiled.h"
#include "3DGrid.h"



ImplementRTTI( Grid3D, sunRenderNode );

Grid3D::Grid3D()
{
	m_pVertData	= NULL;
	m_iGridSize = 90;
	SetName( L"Grid" );
}

Grid3D::~Grid3D()
{
	
}

void Grid3D::Destory()
{
	Safe_Delete( m_pVertData );
}

void Grid3D::CreateNewGrid( void )
{
	assert(NULL != sunGraphicCore::GetCore());

	sunRenderer* pRenderer = sunGraphicCore::GetCore()->GetRenderer();
	assert( pRenderer );
	if( pRenderer->GetRendererID() != RENDERER_DX9 )
	{
		assert(0&& "사용할 수 없는 환경");
	}

	sunRendererDX9* pRenderDX9 = (sunRendererDX9*)pRenderer;
	LPDIRECT3DDEVICE9 pD3DDevice = pRenderDX9->GetDevice();

	int iGridCount = (int)(m_iGridSize * 0.02);
	int iVertCount = iGridCount * 8 + 12;

	Safe_Delete( m_pVertData );
	m_pVertData = new sunVertexData;
	m_pVertData->m_iVertexCount = iVertCount;
	m_pVertData->m_iVertexSize = sizeof(stGridVtx);

	stGridVtx*	pGrid = NULL;
	float		fMax = 10000.0f;

	HRESULT hr;
	if( FAILED( hr = pD3DDevice->CreateVertexBuffer( iVertCount * sizeof(stGridVtx), 0, 0, 
		D3DPOOL_MANAGED, &m_pVertData->m_pVB, NULL) ) )
	{
		assert(0&& "3DGrid::Create - Failed to CreateVertexBuffer!!");
		return;
	}

	if( FAILED( hr = m_pVertData->m_pVB->Lock(0, iVertCount * sizeof(stGridVtx), (void**)&pGrid, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD) ) )  
	{
		assert(0&& "3DGrid::Create - Failed to Lock Vertex Buffer!!");
		return;
	}


	pGrid[ 0] = stGridVtx(-fMax,     0,     0, 0xFFFF0000);
	pGrid[ 1] = stGridVtx(    0,     0,     0, 0xFFFF0000);
	pGrid[ 2] = stGridVtx(    0,     0,     0, 0xFFFF0000);
	pGrid[ 3] = stGridVtx( fMax,     0,     0, 0xFFFF0000);

	pGrid[ 4] = stGridVtx(    0, -fMax,     0, 0xFF00FF00);
	pGrid[ 5] = stGridVtx(    0,     0,     0, 0xFF00FF00);
	pGrid[ 6] = stGridVtx(    0,     0,     0, 0xFF00FF00);
	pGrid[ 7] = stGridVtx(    0,  fMax,     0, 0xFF00FF00);

	pGrid[ 8] = stGridVtx(    0,     0, -fMax, 0xFF0000FF);
	pGrid[ 9] = stGridVtx(    0,     0,     0, 0xFF0000FF);
	pGrid[10] = stGridVtx(    0,     0,     0, 0xFF0000FF);
	pGrid[11] = stGridVtx(    0,     0,  fMax, 0xFF0000FF);

	DWORD	d1 = 0xFF999999;
	DWORD	d2 = 0xFF555555;

	stGridVtx*	pLine = &pGrid[ 12 ];
	for( int i = 0; i < iGridCount; ++i )
	{
		pLine[ 8*i+0 ] = stGridVtx((float)-m_iGridSize, 0.0f,  50.0f * (i+1), (i%2)? d1 : d2);
		pLine[ 8*i+1 ] = stGridVtx((float) m_iGridSize, 0.0f,  50.0f * (i+1), (i%2)? d1 : d2);
		pLine[ 8*i+2 ] = stGridVtx((float)-m_iGridSize, 0.0f, -50.0f * (i+1), (i%2)? d1 : d2);
		pLine[ 8*i+3 ] = stGridVtx((float) m_iGridSize, 0.0f, -50.0f * (i+1), (i%2)? d1 : d2);

		pLine[ 8*i+4 ] = stGridVtx(  50.0f * (i+1), 0.0f, (float)-m_iGridSize, (i%2)? d1 : d2);
		pLine[ 8*i+5 ] = stGridVtx(  50.0f * (i+1), 0.0f, (float) m_iGridSize, (i%2)? d1 : d2);
		pLine[ 8*i+6 ] = stGridVtx( -50.0f * (i+1), 0.0f, (float)-m_iGridSize, (i%2)? d1 : d2);
		pLine[ 8*i+7 ] = stGridVtx( -50.0f * (i+1), 0.0f, (float) m_iGridSize, (i%2)? d1 : d2);
	}
	m_pVertData->m_pVB->Unlock();

	D3DVERTEXELEMENT9 pVertexElements[] = 
	{ 
		{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,	0 }, 
		{ 0,	12,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_COLOR,		0 }, 
		D3DDECL_END()
	}; 

	if( FAILED( hr = pD3DDevice->CreateVertexDeclaration( pVertexElements, &m_pVertData->m_pVertexDeclaration ) ) ) 
		assert(0&& "C3DGrid::Create - Failed to CreateVertexDeclaration" );	
}

void Grid3D::GetRenderOperation( sunRenderOperation& op )
{
	op.m_bUseIndexes = false;
	op.m_PrimType = D3DPT_LINELIST;
	op.m_pVertexData = m_pVertData;
	op.m_dwPrimCount = m_pVertData->m_iVertexCount / 2;
}

void Grid3D::SetGridSize( int iSize )
{
	if( iSize == 0 || iSize < 100 || iSize % 100 != 0 )
	{
		assert(0&&"C3DGrid::SetGridSize - 잘못된 값을 입력 하였습니다. 입력값을 100단위여야 됨." );	
		return;
	}

	m_iGridSize = iSize;
	CreateNewGrid();
}