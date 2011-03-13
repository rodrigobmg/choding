
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
	
// 	matWorld._42 = rand()%80 - 40;
// 	matWorld._41 = rand()%80 - 40;
	D3DXMatrixTranslation( &matWorld , 0 , 5 , 0 );
	D3DXMatrixRotationX( &matWorld , 2 );
	D3DXMatrixScaling( &matWorld , 2 , 1 , 2 );
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
