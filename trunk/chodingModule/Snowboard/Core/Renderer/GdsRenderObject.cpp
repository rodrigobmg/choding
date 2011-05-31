#include "GdsRenderObject.h"


GdsRenderObject::GdsRenderObject()
: m_iTextureIndex( 0 ) , m_Texture( NULL ) , m_vb( NULL ) , m_ib( NULL ) , m_ePrimitive(D3DPT_TRIANGLELIST)
{
	SetName( OBJECT_RENDEROBJECT );
	ZeroMemory( &m_Material , sizeof(D3DMATERIAL9) );
	m_Material.Diffuse.r = m_Material.Ambient.r = 1.0f;
	m_Material.Diffuse.g = m_Material.Ambient.g = 1.0f;
	m_Material.Diffuse.b = m_Material.Ambient.b = 1.0f;
	m_Material.Diffuse.a = m_Material.Ambient.a = 1.0f;
	D3DXMatrixIdentity( &m_TM );
}

GdsRenderObject::~GdsRenderObject()
{
}

void GdsRenderObject::vRender( LPDIRECT3DDEVICE9 device )
{
	if ( !device )
		return;

	device->SetTransform( D3DTS_WORLD , &m_TM );

	device->SetMaterial( &m_Material );
	if( m_Texture )
		device->SetTexture( m_iTextureIndex , m_Texture );		

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
	
	if ( m_vb )
	{
		device->SetFVF( m_FVF );
		device->SetStreamSource( 0 , m_vb , 0 , m_VertexSize );
	}
	if ( m_ib )
	{
		device->SetIndices( m_ib );
		device->DrawIndexedPrimitive( m_ePrimitive , 0 , m_iStartVertexIndex , m_Vertex_Maxcount , m_iStartIndex , m_Index_Maxcount );
	}	
}

