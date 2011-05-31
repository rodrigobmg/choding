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

	// 			D3DXVECTOR3 vecDir;									/// 방향성 광원(directional light)이 향할 빛의 방향
	// 			D3DLIGHT9 light;									/// 광원 구조체
	// 			ZeroMemory( &light, sizeof(D3DLIGHT9) );			/// 구조체를 0으로 지운다.
	// 			light.Type       = D3DLIGHT_DIRECTIONAL;			/// 광원의 종류(점 광원,방향성 광원,스포트라이트)
	// 			light.Diffuse.r  = 1.0f;							/// 광원의 색깔과 밝기
	// 			light.Diffuse.g  = 1.0f;
	// 			light.Diffuse.b  = 0.0f;
	// 			vecDir = D3DXVECTOR3( 1, 1, 1 );					/// 광원 고정
	// 			vecDir = D3DXVECTOR3(cosf(GetTickCount()/350.0f),	/// 광원 회전
	// 				1.0f,
	// 				sinf(GetTickCount()/350.0f) );
	// 			D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );	/// 광원의 방향을 단위벡터로 만든다.
	// 			light.Range       = 1000.0f;									/// 광원이 다다를수 있는 최대거리
	// 			device->SetLight( 0, &light );							/// 디바이스에 0번 광원 설치
	// 			device->LightEnable( 0, TRUE );							/// 0번 광원을 켠다
//	device->SetRenderState( D3DRS_LIGHTING, FALSE );			/// 광원설정을 켠다
	// 
	// 			device->SetRenderState( D3DRS_AMBIENT, 0x00909090 );		/// 환경광원(ambient light)의 값 설정
	
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

