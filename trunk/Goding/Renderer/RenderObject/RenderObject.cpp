
#include "stdafx.h"
#include "RenderObject.h"

RenderObject::RenderObject():
m_pEffect(NULL),
m_indexBuffer(NULL),
m_vertexBuffer(NULL),
m_VertexDeclaration(NULL)
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

	pDevice->SetVertexDeclaration( m_VertexDeclaration );

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