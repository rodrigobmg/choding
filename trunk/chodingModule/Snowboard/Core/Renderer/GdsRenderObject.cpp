#include "GdsRenderObject.h"


GdsRenderObject::GdsRenderObject()
{
	SetName( OBJECT_RENDEROBJECT );
	ZeroMemory( &m_Material , sizeof(D3DMATERIAL9) );
	m_Material.Diffuse.r = m_Material.Ambient.r = 1.0f;
	m_Material.Diffuse.g = m_Material.Ambient.g = 1.0f;
	m_Material.Diffuse.b = m_Material.Ambient.b = 1.0f;
	m_Material.Diffuse.a = m_Material.Ambient.a = 1.0f;
	m_iTextureIndex = 0;
}


void GdsRenderObject::vRender( LPDIRECT3DDEVICE9 device )
{
	device->SetMaterial( &m_Material );
	device->SetTexture( m_iTextureIndex , m_Texture );		
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
//	device->SetRenderState( D3DRS_LIGHTING, FALSE );			/// ���������� �Ҵ�
	// 
	// 			device->SetRenderState( D3DRS_AMBIENT, 0x00909090 );		/// ȯ�汤��(ambient light)�� �� ����


	device->SetFVF( m_FVF );
	device->SetStreamSource( 0 , m_vb , 0 , m_VertexSize );
	device->SetIndices( m_ib );
	device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , 0 , m_Vertex_Maxcount , 0 , m_Index_Maxcount );
}
