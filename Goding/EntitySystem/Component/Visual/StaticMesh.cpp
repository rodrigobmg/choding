#include "stdafx.h"
#include "StaticMesh.h"

StaticMesh::StaticMesh():
m_bLoaded( false ),
m_indexBuffer(NULL),
m_vertexBuffer(NULL),
m_pEffect(NULL)
{
	m_ID = STATIC_MESH;
}


StaticMesh::~StaticMesh(void)
{
	SAFE_RELEASE( m_indexBuffer );
	SAFE_RELEASE( m_vertexBuffer );
	SAFE_RELEASE( m_pEffect );
}


void StaticMesh::onDeviceReset( D3DDevice* pDevice )
{
	if ( m_bLoaded == true )
		return;

	void* BufferMemory;

	UINT bSize = Vertices.size() * sizeof( Vertex );
	HRESULT r = pDevice->CreateVertexBuffer(bSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_vertexBuffer, 0);

	if(r == D3D_OK)
	{
		m_vertexBuffer->Lock(0, bSize, &BufferMemory, 0);
		memcpy(BufferMemory, &Vertices[0], bSize);
		m_vertexBuffer->Unlock();
	}

	bSize = sizeof(uint32) * Indices.size();
	r = pDevice->CreateIndexBuffer(bSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_indexBuffer, 0);

	if(r == D3D_OK)
	{
		m_indexBuffer->Lock(0, bSize, &BufferMemory, 0);
		memcpy(BufferMemory, &Indices[0], bSize);
		m_indexBuffer->Unlock();
	}

	D3DXCreateEffectFromFile( pDevice , L"..//asset//Static.fx", NULL, NULL, NULL, NULL, &m_pEffect, NULL);


	D3DVERTEXELEMENT9 ve[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
		{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
		D3DDECL_END()
	};

	pDevice->CreateVertexDeclaration(ve, &m_VertexDeclaration);


	if( m_vertexBuffer && m_indexBuffer && m_pEffect && m_VertexDeclaration )
	{
		m_bLoaded = true;
	}


}

void StaticMesh::onDeviceLost()
{
	m_bLoaded = false;

	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_VertexDeclaration);
}

