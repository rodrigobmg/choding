
#include "GdsMeshNode.h"
#include "Resource\Type\GdsResMD2.h"

GdsMeshNode::GdsMeshNode()
{
	SetName( OBJECT_NODE_MESH );
}

GdsMeshNode::~GdsMeshNode()
{
}

void GdsMeshNode::vInitGeometry()
{
// 	if( FAILED( GetDevice()->CreateVertexBuffer( Size * sizeof(MD2_VERTEX), 0, m_dFVF, D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
// 	{ return E_FAIL; }
// 
// 	void *pVertices;
// 	if( FAILED( m_pVB->Lock( 0, Size * sizeof(MD2_VERTEX), (void**)&pVertices, 0 ) ) )
// 	{ return E_FAIL; }
// 	memcpy( pVertices, Vertices, Size * sizeof(MD2_VERTEX) );
// 	m_pVB->Unlock();

// 	D3DXMATRIXA16 matWorld;
// 	D3DXMatrixIdentity( &matWorld );
// 	z += 3.0f;
// 	matWorld._42 = z;
// 	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
}

void GdsMeshNode::vRender()
{
	if ( GetDevice() == NULL )
		return;

 	GetDevice()->SetTexture( 0, m_pTexture );
	GetDevice()->SetStreamSource( 0, m_pVB, 0, sizeof(GdsResMD2::MD2_VERTEX) );	
 	GetDevice()->SetFVF( m_dFVF );
 	GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_uPrimitive );
}

void GdsMeshNode::vClear()
{

}