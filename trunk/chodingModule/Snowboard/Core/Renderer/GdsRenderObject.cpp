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
 	device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	/// 0번 텍스처 스테이지의 확대 필터
 	device->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		/// 0번 텍스처 : 0번 텍스처 인덱스 사용 
 	device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
 	device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
 	device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

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


	device->SetFVF( m_FVF );
	device->SetStreamSource( 0 , m_vb , 0 , m_VertexSize );
	device->SetIndices( m_ib );
	device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , 0 , m_Vertex_Maxcount , 0 , m_Index_Maxcount );
}
