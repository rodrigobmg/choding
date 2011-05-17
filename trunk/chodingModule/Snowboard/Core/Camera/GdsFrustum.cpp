#include "GdsFrustum.h"

GdsFrustum::GdsFrustum( float fLeft , float fRight , float fBottom , float fTop , float fNear , float fFar , bool bOrtho /*= false*/ )
{
	SetName( OBJECT_FRUSTUM );

	m_bOrtho = bOrtho;
	
	m_fZnear = fNear;
	m_fFar = fFar;
	m_fLeft = fLeft;
	m_fRight = fRight;
	m_fBottom = fBottom;
	m_fTop = fTop;

}

void GdsFrustum::UpdatePlane( const D3DXMATRIX& viewprojMat )
{
	int				i;
	D3DXMATRIXA16	matInv;

	// 투영행렬까지 거치면 모든 3차원 월드좌표의 점은 (-1,-1,0) ~ (1,1,1)사이의 값으로 바뀐다.
	// m_vtx에 이 동차공간의 경계값을 넣어둔다.
	m_vtx[0].x = -1.0f;	m_vtx[0].y = -1.0f;	m_vtx[0].z = 0.0f;
	m_vtx[1].x =  1.0f;	m_vtx[1].y = -1.0f;	m_vtx[1].z = 0.0f;
	m_vtx[2].x =  1.0f;	m_vtx[2].y = -1.0f;	m_vtx[2].z = 1.0f;
	m_vtx[3].x = -1.0f;	m_vtx[3].y = -1.0f;	m_vtx[3].z = 1.0f;
	m_vtx[4].x = -1.0f;	m_vtx[4].y =  1.0f;	m_vtx[4].z = 0.0f;
	m_vtx[5].x =  1.0f;	m_vtx[5].y =  1.0f;	m_vtx[5].z = 0.0f;
	m_vtx[6].x =  1.0f;	m_vtx[6].y =  1.0f;	m_vtx[6].z = 1.0f;
	m_vtx[7].x = -1.0f;	m_vtx[7].y =  1.0f;	m_vtx[7].z = 1.0f;

	// view * proj의 역행렬을 구한다.
	D3DXMatrixInverse(&matInv, NULL, &viewprojMat );

	// Vertex_최종 = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj 인데,
	// Vertex_world = Vertex_local * Matrix_world이므로,
	// Vertex_최종 = Vertex_world * Matrix_view * Matrix_Proj 이다.
	// Vertex_최종 = Vertex_world * ( Matrix_view * Matrix_Proj ) 에서
	// 역행렬( Matrix_view * Matrix_Proj )^-1를 양변에 곱하면
	// Vertex_최종 * 역행렬( Matrix_view * Matrix_Proj )^-1 = Vertex_World 가 된다.
	// 그러므로, m_vtx * matInv = Vertex_world가 되어, 월드좌표계의 프러스텀 좌표를 얻을 수 있다.
	for( i = 0; i < 8; i++ )
		D3DXVec3TransformCoord( &m_vtx[i], &m_vtx[i], &matInv );


	// 얻어진 월드좌표로 프러스텀 평면을 만든다
	// 벡터가 프러스텀 안쪽에서 바깥쪽으로 나가는 평면들이다.
	D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);	// 상 평면(top)
	D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);	// 하 평면(bottom)
	D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);	// 근 평면(near)
	D3DXPlaneFromPoints(&m_plane[3], m_vtx+2, m_vtx+6, m_vtx+7);	// 원 평면(far)
	D3DXPlaneFromPoints(&m_plane[4], m_vtx  , m_vtx+3, m_vtx+7);	// 좌 평면(left)
	D3DXPlaneFromPoints(&m_plane[5], m_vtx+1, m_vtx+5, m_vtx+6);	// 우 평면(right)
}

bool GdsFrustum::VertexIsInFrustum( const D3DXVECTOR3& v )
{
	for( int i = 0; i < 6; ++i )
	{
		float fDist = D3DXPlaneDotCoord( &m_plane[i], &v );
		if( fDist <= 5.0f )
			return true;
	}

	return false;
}

bool GdsFrustum::SphereIsInFrustum( const D3DXVECTOR3& v, float fradius ) 
{
	for( int  i = 0 ; i < 6; ++i )
	{
		float fDist = D3DXPlaneDotCoord( &m_plane[i], &v );
		if( fDist <= ( fradius + 5.0f ) )
			return true;
	}

	return false;
}