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
 			device->SetRenderState( D3DRS_LIGHTING, FALSE );			/// 광원설정을 켠다
// 
// 			device->SetRenderState( D3DRS_AMBIENT, 0x00909090 );		/// 환경광원(ambient light)의 값 설정

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

