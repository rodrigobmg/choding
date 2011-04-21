#include "GdsMesh.h"

GdsMesh::GdsMesh():
m_bDrawAxis( false ),
m_ib(NULL),
m_vb(NULL),
m_Material( (GdsMaterial*)(NULL) )
{

}

GdsMesh::~GdsMesh()
{
	SAFE_RELEASE( m_ib );
	SAFE_RELEASE( m_vb );
}

void GdsMesh::vRender( LPDIRECT3DDEVICE9 device )
{
	if ( m_bDrawAxis )
		drawAxis( device );

	if ( m_vb || m_ib )
	{
		device->SetTransform( D3DTS_WORLD , &m_DXmatWorld );
		device->SetStreamSource( 0 , m_vb , 0 , m_VertexSize );
		device->SetIndices( m_ib );
		device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , 0 , m_Vertex_Maxcount , 0 , m_Index_Maxcount );
	}
	//device->SetMaterial( &m_dxMaterial );		
}

void GdsMesh::drawAxis( LPDIRECT3DDEVICE9 device )
{
 	D3DXMATRIXA16 matWorld;
 	D3DXMATRIXA16 matView;
 	D3DXMATRIXA16 matProj;
 	device->GetTransform( D3DTS_WORLD , &matWorld );
 	device->GetTransform( D3DTS_VIEW  , &matView );
 	device->GetTransform( D3DTS_PROJECTION , &matProj );
 
 	D3DXVECTOR3 axisX[2];
 	axisX[0].x = 0.0f; axisX[0].y = 0.0f; axisX[0].z = 0.0f;
 	axisX[1].x = 10.0f; axisX[1].y = 0.0f; axisX[1].z = 0.0f;
 
 	D3DXVECTOR3 axisY[2];
 	axisY[0].x = 0.0f; axisY[0].y = 0.0f; axisY[0].z = 0.0f;
 	axisY[1].x = 0.0f; axisY[1].y = 10.0f; axisY[1].z = 0.0f;
 
 	D3DXVECTOR3 axisZ[2];
 	axisZ[0].x = 0.0f; axisZ[0].y = 0.0f; axisZ[0].z = 0.0f;
 	axisZ[1].x = 0.0f; axisZ[1].y = 0.0f; axisZ[1].z = 10.0f;
 
 	//D3DXMatrixIdentity( &matWorld );
 
 	ID3DXLine* Line;
 	D3DXCreateLine( device , &Line );
 	Line->SetWidth( 1 );
 	Line->SetAntialias( true );
 	Line->Begin();
 	Line->DrawTransform( axisX , 2, &(matWorld*matView*matProj), D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ));
 	Line->DrawTransform( axisY , 2, &(matWorld*matView*matProj), D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
 	Line->DrawTransform( axisZ , 2, &(matWorld*matView*matProj), D3DXCOLOR( 0.0f , 0.0f , 1.0f , 1.0f ));
 	Line->End();
 	Line->Release();
}

