#include "Precompiled.h"
#include "sunMesh.h"



ImplementBPool(sunMesh);

sunMesh::sunMesh()
{
	m_pVertexData = NULL;
	m_pIndexData  = NULL;

}

sunMesh::~sunMesh()
{
	Safe_Delete(m_pIndexData);
	Safe_Delete(m_pIndexData);
}


bool sunMesh::Load(const tstring& strFileName)
{
	sunStream::SetFileName(strFileName);

	return true;
}

int sunMesh::CreateVIB()
{
	if(0 == (int)m_vecVertex.size())
		return 0;

	m_pVertexData = new sunVertexData;
	assert(NULL != m_pVertexData);

	assert(sunGraphicCore::GetCore());
	sunRenderer* pRenderer = sunGraphicCore::GetCore()->GetRenderer();

	sunRendererDX9* pRenderDX9 = static_cast<sunRendererDX9*>(pRenderer);
	LPDIRECT3DDEVICE9 pD3DDevice = pRenderDX9->GetDevice();

	assert(pRenderer);

	if(pRenderer->GetRendererID() != RENDERER_DX9)
	{
		assert(0 && "CreateVIB 사용할 수 없는 환경");
	}

	HRESULT hr;

	//버텍스 정의 설정.
	D3DVERTEXELEMENT9 pVertexElements[] =
	{

		{0,0,  D3DDECLTYPE_FLOAT3,	 D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0,12, D3DDECLTYPE_FLOAT3,	 D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
		{0,24, D3DDECLTYPE_FLOAT2,	 D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,   0},
//		{0,24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_COLOR,	   0},
		D3DDECL_END()
	};

	m_pVertexData->m_iVertexSize = 32;
	if( FAILED( hr = pD3DDevice->CreateVertexDeclaration(pVertexElements, &m_pVertexData->m_pVertexDeclaration)) )
	{
		assert(0 && "CreateVIB - Failed CreateVertexDeclaration");
	}
		
	// 정점 버퍼 생성
	m_pVertexData->m_iVertexCount = static_cast<int>(m_vecVertex.size());
	if( FAILED( pD3DDevice->CreateVertexBuffer(m_pVertexData->m_iVertexCount * m_pVertexData->m_iVertexSize, 0, 0,
		 D3DPOOL_MANAGED, &m_pVertexData->m_pVB,NULL)))
	{
		assert(0 && "CreateVertexBuffer is Failed");	
		return 0;
	}

	// 정점 정보를 버퍼에 복사.
	unsigned char* pVertex = NULL;
	if( FAILED( m_pVertexData->m_pVB->Lock(0, m_pVertexData->m_iVertexCount * m_pVertexData->m_iVertexSize, (void**)&pVertex, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD )))
	{
		assert(0 && "VB Lock is Failed.");
		return 0;
	}

	for( UINT i = 0; i < m_pVertexData->m_iVertexCount; ++i)
	{
		*((D3DXVECTOR3*)(pVertex)) = m_vecVertex[i].vPosition; pVertex += sizeof(D3DXVECTOR3);
		*((D3DXVECTOR3*)(pVertex)) = m_vecVertex[i].vNormal	 ; pVertex += sizeof(D3DXVECTOR3);
		*((D3DXVECTOR2*)(pVertex)) = m_vecVertex[i].vUV	 ; pVertex += sizeof(D3DXVECTOR2);
	//	*((DWORD*)(pVertex))	   = m_vecVertex[i].dwColor	 ; pVertex += sizeof(DWORD);
	}		
	
	m_pVertexData->m_pVB->Unlock();
	
	m_pIndexData = new sunIndexData;
	assert(NULL != m_pIndexData);

	// 인덱스 버퍼 생성
	m_pIndexData->m_iIndexCount = (int)m_vecIndex.size();
	
	if( FAILED( pD3DDevice->CreateIndexBuffer( m_pIndexData->m_iIndexCount * (sizeof(WORD)*3), 0,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexData->m_pIB, NULL) ) ) 
	{
		assert(0 && "CreateIndexBuffer is Failed");
		return 0;
	}	

	unsigned char* pIndex = NULL;

	if( FAILED(m_pIndexData->m_pIB->Lock(0, m_pIndexData->m_iIndexCount * (sizeof(WORD)*3), (void**)&pIndex, 0 ) ) )
	{
		assert(0 && " IB Lock is Failed");
		return 0;
	}
	
	for(UINT i = 0; i < m_pIndexData->m_iIndexCount; ++i)
	{
		*((WORD*)(pIndex)) = (WORD)m_vecIndex[i].wIndex[0]; pIndex += sizeof(WORD);
		*((WORD*)(pIndex)) = (WORD)m_vecIndex[i].wIndex[1]; pIndex += sizeof(WORD);
		*((WORD*)(pIndex)) = (WORD)m_vecIndex[i].wIndex[2]; pIndex += sizeof(WORD);
	}
	
	m_pIndexData->m_pIB->Unlock();
	
	m_vecVertex.clear();

	return true;
}


sunVertexData* sunMesh::GetVertexData()
{
	return m_pVertexData;
}

sunIndexData* sunMesh::GetindexData()
{
	return 	m_pIndexData;
}

void sunMesh::SetVertexInfo(vector<stVector3f>* vecVertex, vector<stVector3f>* vecVertexNormal, vector<stVector2f>* vecTVertex)
{
	assert(vecVertex);
	assert(vecVertexNormal);
	

	int iVectexSize		  = static_cast<int>(vecVertex->size());
	int iVectexNormalSize = static_cast<int>(vecVertexNormal->size());
	int iTVertexSize	  = static_cast<int>(vecTVertex->size());

	if(iVectexSize != iVectexNormalSize)
		assert(0 && "VertexSize != VectexNormalSize");



	for(int i=0; i < (int)vecVertex->size(); ++i)
	{
		stVertex kVertex;
		kVertex.vPosition = D3DXVECTOR3((*vecVertex)[i].x , (*vecVertex)[i].y, (*vecVertex)[i].z);
		kVertex.vNormal   = D3DXVECTOR3((*vecVertexNormal)[i].x, (*vecVertexNormal)[i].y, (*vecVertexNormal)[i].z);
		if(iTVertexSize > 0)
			kVertex.vUV		  = D3DXVECTOR2((*vecTVertex)[i].x , (*vecTVertex)[i].y);
		m_vecVertex.push_back(kVertex);
	}
}

void sunMesh::SetIndexInfo(vector<stVector3i>* vecIndex)
{
	assert(vecIndex);
		
	int iVecSize = static_cast<int>(vecIndex->size());

	for(int i=0; i < iVecSize; ++i)
	{
		stIndex kIndex;
		kIndex.wIndex[0] = (*vecIndex)[i].x;
		kIndex.wIndex[1] = (*vecIndex)[i].y;
		kIndex.wIndex[2] = (*vecIndex)[i].z;

		m_vecIndex.push_back(kIndex);
	}


}
