
#include "stdafx.h"
#include "RenderObject.h"

RenderObject::RenderObject():
m_pEffect(NULL),
m_indexBuffer(NULL),
m_vertexBuffer(NULL)
{

}

RenderObject::~RenderObject()
{

}

void RenderObject::Draw( D3DDevice* pDevice )
{
	if( pDevice == NULL )
		return;

	if ( m_pEffect == NULL )
		return;

	//Set vertex buffer
	pDevice->SetStreamSource(0, m_vertexBuffer, 0, STRIDE_SIZE);

	//Set index buffer
	pDevice->SetIndices(m_indexBuffer);

	//Set parameters
	m_pEffect->SetMatrix("World", &m_world );

	//Begin drawing
	m_pEffect->Begin(NULL, NULL);
	m_pEffect->BeginPass(0);

	//Draw
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_indexCount , 0, m_indexCount / 3 );

	//End drawing
	m_pEffect->EndPass();
	m_pEffect->End();
}

/*
void RenderObject::onDeviceReset( D3DDevice* pDevice )
{
	void* BufferMemory;

	UINT bSize = m_VerticesSize * STRIDE_SIZE;
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
}

void RenderObject::onDeviceLost()
{
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_vertexBuffer);
}*/