
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
	if ( GetDevice() == NULL )
		return;

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	GetDevice()->SetTransform( D3DTS_WORLD, &matWorld );
}

void GdsMeshNode::vRender()
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
