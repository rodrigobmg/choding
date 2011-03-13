
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

	//Triangle();

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	GetDevice()->SetTransform( D3DTS_WORLD, &matWorld );

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f,-100.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	GetDevice()->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 1000.0f );
	GetDevice()->SetTransform( D3DTS_PROJECTION, &matProj );
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

// 	GetDevice()->SetStreamSource( 0, g_pVB , 0, sizeof(CUSTOMVERTEX) );	
// 	GetDevice()->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE );
// 	GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
}

void GdsMeshNode::vClear()
{

}

void GdsMeshNode::Triangle()
{
	CUSTOMVERTEX g_Vertices[] = 
	{
		{ -1.0f , -1.0f , 0.0f , 0xffff0000, },
		{ 1.0f , -1.0f , 0.0f , 0xff0000ff, },
		{ 0.0f , 1.0f , 0.0f , 0xffffffff, },
	};

	if( FAILED( GetDevice()->CreateVertexBuffer( 3*sizeof( CUSTOMVERTEX ) , 
												0, D3DFVF_XYZ|D3DFVF_DIFFUSE ,
												D3DPOOL_DEFAULT ,
												&g_pVB , NULL )))
	{
		return;
	}

	VOID* pVertices;
	if( FAILED( g_pVB->Lock( 0, sizeof( g_Vertices ) , (void**)&pVertices , 0 )))
		return;

	memcpy( pVertices , g_Vertices , sizeof( g_Vertices ) );
	g_pVB->Unlock();
}
