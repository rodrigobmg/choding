#include "GdsRenderObject.h"


GdsRenderObject::GdsRenderObject()
: m_iTextureIndex( 0 ) , m_Texture( NULL ) , m_vb( NULL ) , m_ib( NULL ) , m_bShowAxis( false ) , m_bShowBox( false )
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
	SAFE_RELEASE( m_ib );
	SAFE_RELEASE( m_vb );
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
		device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , m_iStartVertexIndex , m_Vertex_Maxcount , m_iStartIndex , m_Index_Maxcount );
	}	

	if ( m_bShowAxis )
		drawAxis( device );
	if ( m_bShowBox )
		drawBox( device );
}

void GdsRenderObject::drawAxis( LPDIRECT3DDEVICE9 device )
{
	if ( device == NULL )
		return;

	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;
	device->GetTransform( D3DTS_WORLD , &matWorld );
	device->GetTransform( D3DTS_VIEW  , &matView );
	device->GetTransform( D3DTS_PROJECTION , &matProj );
	D3DXMATRIXA16 mat = matWorld*matView*matProj;

	D3DXVECTOR3 lineLBN[2];
	lineLBN[0].x = 0.0f; lineLBN[0].y = 0.0f; lineLBN[0].z = 0.0f;
	lineLBN[1].x = 10.0f; lineLBN[1].y = 0.0f; lineLBN[1].z = 0.0f;

	D3DXVECTOR3 lineLTN[2];
	lineLTN[0].x = 0.0f; lineLTN[0].y = 0.0f; lineLTN[0].z = 0.0f;
	lineLTN[1].x = 0.0f; lineLTN[1].y = 10.0f; lineLTN[1].z = 0.0f;

	D3DXVECTOR3 lineLBF[2];
	lineLBF[0].x = 0.0f; lineLBF[0].y = 0.0f; lineLBF[0].z = 0.0f;
	lineLBF[1].x = 0.0f; lineLBF[1].y = 0.0f; lineLBF[1].z = 10.0f;


	ID3DXLine* Line;
	D3DXCreateLine( device , &Line );
	Line->SetWidth( 1 );
	Line->SetAntialias( true );
	Line->Begin();
	Line->DrawTransform( lineLBN , 2, &mat, D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLBF , 2, &mat, D3DXCOLOR( 0.0f , 0.0f , 1.0f , 1.0f ));
	Line->End();
	Line->Release();
}

void GdsRenderObject::drawBox( LPDIRECT3DDEVICE9 device )
{
	if ( device == NULL )
		return;

	D3DXVECTOR3 minPos( 0,0,0 );
	D3DXVECTOR3 maxPos( 0,0,0 );
	VOID*	pVB;
	if (  SUCCEEDED( m_vb->Lock( 0 , GetVertexMaxCount() * sizeof( GDSVERTEX ) , (void**)&pVB , 0 ) ) )
	{		
		for ( int i=0 ; i < GetVertexMaxCount() ; i++ )
		{				
			GDSVERTEX* v = (GDSVERTEX*)pVB + i;

			if ( minPos.x > v->p.x )
				minPos.x = v->p.x;

			if ( minPos.y > v->p.y )
				minPos.y = v->p.y;

			if ( minPos.z > v->p.z )
				minPos.z = v->p.z;

			if ( maxPos.x < v->p.x )
				maxPos.x = v->p.x;
			if ( maxPos.y < v->p.y )
				maxPos.y = v->p.y;
			if ( maxPos.z < v->p.z )
				maxPos.z = v->p.z;
		}
		m_vb->Unlock();
	}


	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;
	device->GetTransform( D3DTS_WORLD , &matWorld );
	device->GetTransform( D3DTS_VIEW  , &matView );
	device->GetTransform( D3DTS_PROJECTION , &matProj );
	D3DXMATRIXA16 mat = matWorld*matView*matProj;

	D3DXVECTOR3 lineLBN[2];
	lineLBN[0] = minPos;
	lineLBN[1].x = minPos.x; lineLBN[1].y = maxPos.y; lineLBN[1].z = minPos.z;

	D3DXVECTOR3 lineLTN[2];
	lineLTN[0] = lineLBN[1];
	lineLTN[1].x = minPos.x; lineLTN[1].y = maxPos.y; lineLTN[1].z = maxPos.z;

	D3DXVECTOR3 lineLBF[2];
	lineLBF[0] = lineLTN[1];
	lineLBF[1].x = minPos.x; lineLBF[1].y = minPos.y; lineLBF[1].z = maxPos.z;

	D3DXVECTOR3 lineLTF[2];
	lineLTF[0] = lineLBF[1];
	lineLTF[1] = minPos;

	D3DXVECTOR3 lineRBN[2];
	lineRBN[0].x = maxPos.x; lineRBN[0].y = minPos.y; lineRBN[0].z = minPos.z;
	lineRBN[1].x = maxPos.x; lineRBN[1].y = maxPos.y; lineRBN[1].z = minPos.z;

	D3DXVECTOR3 lineRTN[2];
	lineRTN[0] = lineRBN[1];
	lineRTN[1].x = maxPos.x; lineRTN[1].y = maxPos.y; lineRTN[1].z = maxPos.z;

	D3DXVECTOR3 lineRBF[2];
	lineRBF[0] = lineRTN[1];
	lineRBF[1].x = maxPos.x; lineRBF[1].y = minPos.y; lineRBF[1].z = maxPos.z;

	D3DXVECTOR3 lineRTF[2];
	lineRTF[0] = lineRBF[1];
	lineRTF[1] = lineRBN[0];

	D3DXVECTOR3 lineBN[2];
	lineBN[0] = minPos;
	lineBN[1].x = maxPos.x; lineBN[1].y = minPos.y; lineBN[1].z = minPos.z;

	D3DXVECTOR3 lineTN[2];
	lineTN[0].x = minPos.x; lineTN[0].y = maxPos.y; lineTN[0].z = minPos.z;
	lineTN[1].x = maxPos.x; lineTN[1].y = maxPos.y; lineTN[1].z = minPos.z;

	D3DXVECTOR3 lineBF[2];
	lineBF[0].x = minPos.x; lineBF[0].y = minPos.y; lineBF[0].z = maxPos.z;
	lineBF[1].x = maxPos.x; lineBF[1].y = minPos.y; lineBF[1].z = maxPos.z;

	D3DXVECTOR3 lineTF[2];
	lineBF[0].x = minPos.x; lineBF[0].y = maxPos.y; lineBF[0].z = maxPos.z;
	lineBF[1] = maxPos;

	ID3DXLine* Line;
	D3DXCreateLine( device , &Line );
	Line->SetWidth( 1 );
	Line->SetAntialias( true );
	Line->Begin();
	Line->DrawTransform( lineLBN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLBF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLTF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRBN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRBF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRTF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineBN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineBF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineTF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));

	Line->End();
	Line->Release();	
}
