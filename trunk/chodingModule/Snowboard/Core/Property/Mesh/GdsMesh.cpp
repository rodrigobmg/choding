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
	device->SetTransform( D3DTS_WORLD , &m_DXmatWorld );

	if ( m_bDrawAxis )
		drawAxis( device );

	if ( m_vb || m_ib )
	{
		if ( m_Material )
		{
			device->SetMaterial( &m_Material->GetDxMaterial() );
			device->SetTexture( 0 , m_Material->GetTexture()->Get() );		
			device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	/// 0�� �ؽ�ó ���������� Ȯ�� ����
			device->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		/// 0�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���

			device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
			device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

// 			D3DXVECTOR3 vecDir;									/// ���⼺ ����(directional light)�� ���� ���� ����
// 			D3DLIGHT9 light;									/// ���� ����ü
// 			ZeroMemory( &light, sizeof(D3DLIGHT9) );			/// ����ü�� 0���� �����.
// 			light.Type       = D3DLIGHT_DIRECTIONAL;			/// ������ ����(�� ����,���⼺ ����,����Ʈ����Ʈ)
// 			light.Diffuse.r  = 1.0f;							/// ������ ����� ���
// 			light.Diffuse.g  = 1.0f;
// 			light.Diffuse.b  = 0.0f;
// 			vecDir = D3DXVECTOR3( 1, 1, 1 );					/// ���� ����
// 			vecDir = D3DXVECTOR3(cosf(GetTickCount()/350.0f),	/// ���� ȸ��
// 				1.0f,
// 				sinf(GetTickCount()/350.0f) );
// 			D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );	/// ������ ������ �������ͷ� �����.
// 			light.Range       = 1000.0f;									/// ������ �ٴٸ��� �ִ� �ִ�Ÿ�
// 			device->SetLight( 0, &light );							/// ����̽��� 0�� ���� ��ġ
// 			device->LightEnable( 0, TRUE );							/// 0�� ������ �Ҵ�
 			device->SetRenderState( D3DRS_LIGHTING, FALSE );			/// ���������� �Ҵ�
// 
// 			device->SetRenderState( D3DRS_AMBIENT, 0x00909090 );		/// ȯ�汤��(ambient light)�� �� ����

		}

		device->SetFVF( m_FVF );
		device->SetStreamSource( 0 , m_vb , 0 , m_VertexSize );
		device->SetIndices( m_ib );
		device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , 0 , m_Vertex_Maxcount , 0 , m_Index_Maxcount );

		
	}
	//device->SetMaterial( &m_dxMaterial );		
}

void GdsMesh::drawAxis( LPDIRECT3DDEVICE9 device )
{
 	D3DXMATRIX matWorld;
 	D3DXMATRIX matView;
 	D3DXMATRIX matProj;
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

