
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
//  	z += 3.0f;
//  	matWorld._42 = z;
 	GetDevice()->SetTransform( D3DTS_WORLD, &matWorld );
}

void GdsMeshNode::vRender()
{
	if ( GetDevice() == NULL )
		return;

	LPDIRECT3DVERTEXBUFFER9 vb;
	LPDIRECT3DTEXTURE9 tex;
	vb = GetVB();
	tex = GetTexture();
	DWORD fvf = GetFVF();
	int prim = GetPrimitive();
  	GetDevice()->SetTexture( 0, tex );
 	GetDevice()->SetStreamSource( 0, vb, 0, sizeof(GdsResMD2::MD2_VERTEX) );	
  	GetDevice()->SetFVF( fvf );
  	GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, prim );
}

void GdsMeshNode::vClear()
{

}

void GdsMeshNode::SetVB( LPDIRECT3DVERTEXBUFFER9 vb )
{

}

LPDIRECT3DVERTEXBUFFER9 GdsMeshNode::GetVB()
{
	return boost::shared_dynamic_cast< GdsResMD2 >( m_pResource )->GetVB();
}

void GdsMeshNode::SetFVF( DWORD flag )
{

}

DWORD GdsMeshNode::GetFVF()
{
	return boost::shared_dynamic_cast< GdsResMD2 >( m_pResource )->GetFVF();
}

void GdsMeshNode::SetTexture( LPDIRECT3DTEXTURE9 texture )
{

}

LPDIRECT3DTEXTURE9 GdsMeshNode::GetTexture()
{
	return boost::shared_dynamic_cast< GdsResMD2 >(m_pResource)->GetTexture();
}

void GdsMeshNode::SetPrimitive( UINT uPrimitive )
{

}

UINT GdsMeshNode::GetPrimitive()
{
	GdsResMD2Ptr p = boost::shared_dynamic_cast< GdsResMD2 >(m_pResource);
	return p->GetPrimitive();
}
