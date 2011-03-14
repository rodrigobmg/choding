
#include "GdsMeshNode.h"
#include "Resource\Type\GdsResMD2.h"

GdsMeshNode::GdsMeshNode()
{
	SetName( OBJECT_NODE_MESH );
}

GdsMeshNode::~GdsMeshNode()
{
}

void GdsMeshNode::vInitGeometry( float fElapsedtime )
{
	if ( GetDevice() == NULL )
		return;

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	
	m_matWorld.m_Rotate.GetRow( 0 , matWorld._11 , matWorld._12 , matWorld._13 );
	m_matWorld.m_Rotate.GetRow( 1 , matWorld._21 , matWorld._22 , matWorld._23 );
	m_matWorld.m_Rotate.GetRow( 2 , matWorld._31 , matWorld._32 , matWorld._33 );
	matWorld._41 = m_matWorld.m_Translate[0]; 
	matWorld._42 = m_matWorld.m_Translate[1];
	matWorld._43 = m_matWorld.m_Translate[2];	
	matWorld._11 *= m_matWorld.m_fScale;
	matWorld._22 *= m_matWorld.m_fScale;
	matWorld._33 *= m_matWorld.m_fScale;
	
	GetDevice()->SetTransform( D3DTS_WORLD, &matWorld );
}

void GdsMeshNode::vRender( float fElapsedtime )
{
	if ( GetDevice() == NULL )
		return;
	
	GdsResMD2Ptr pResource = boost::dynamic_pointer_cast< GdsResMD2 >( GetResource() );
	if ( pResource == NULL )
		return;

  	GetDevice()->SetTexture( 0, pResource->GetTexture() );
  	GetDevice()->SetStreamSource( 0, pResource->GetVB() , 0, sizeof(GdsResMD2::MD2_VERTEX) );	
   	GetDevice()->SetFVF( pResource->GetFVF() );
   	GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, pResource->GetPrimitive() );
}

void GdsMeshNode::vClear()
{

}
