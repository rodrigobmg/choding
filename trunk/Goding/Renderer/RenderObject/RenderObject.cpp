
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

void RenderObject::Draw( D3DDevice* pDevice , Camera* camera )
{
	if( pDevice == NULL )
		return;

	if ( m_pEffect == NULL )
		return;
	
	pDevice->SetVertexDeclaration( m_VertexDeclaration );

	m_pEffect->SetMatrix("View", &(camera->View) );
	m_pEffect->SetMatrix("Projection", &(camera->Projection) );
	m_pEffect->SetMatrix("World", &m_world );
	m_pEffect->SetVector("Light" , &m_vecLight );

	pDevice->SetStreamSource(0, m_vertexBuffer, 0, STRIDE_SIZE);
	pDevice->SetIndices(m_indexBuffer);	

	m_pEffect->Begin(NULL, NULL);
	m_pEffect->BeginPass(0);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_indexCount , 0, m_indexCount / 3 );

	m_pEffect->EndPass();
	m_pEffect->End();
}