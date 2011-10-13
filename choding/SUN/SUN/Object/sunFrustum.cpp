#include "Precompiled.h"
#include "sunFrustum.h"


ImplementRTTI( sunFrustum, sunNode );

sunFrustum::sunFrustum()
{
	m_fFOV		= D3DX_PI / 4.0f; // * 0.25f;
	m_fAspect	= 1.333333333333f;
	m_fNearDist = 1.0f;
	m_fFarDist  = 1000.0f;

	ZeroMemory(m_plFrustum, sizeof(m_plFrustum));

	D3DXMatrixIdentity( &m_matProj );
	D3DXMatrixIdentity( &m_matView );

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFOV, m_fAspect, m_fNearDist, m_fFarDist);

}


sunFrustum::~sunFrustum()
{

}

const D3DXMATRIX& sunFrustum::GetProjectionMatrix( void )
{
	return m_matProj;
}


const D3DXMATRIX& sunFrustum::GetViewMatrix( void )
{
	return m_matView;
}

float sunFrustum::GetFarClip( void )
{
	return m_fFarDist;
}


void sunFrustum::UpdateFrustumPlane( void )
{
	D3DXMATRIX matViewProj;
	D3DXMatrixMultiply( &matViewProj, &m_matView, &m_matProj );

	D3DXVECTOR3 vtx[8];

	vtx[0] = D3DXVECTOR3( -1.0f, -1.0f, 0.0f);
	vtx[1] = D3DXVECTOR3(  1.0f, -1.0f, 0.0f);
	vtx[2] = D3DXVECTOR3(  1.0f, -1.0f, 1.0f);
	vtx[3] = D3DXVECTOR3( -1.0f, -1.0f, 1.0f);	
	vtx[4] = D3DXVECTOR3( -1.0f,  1.0f, 0.0f);
	vtx[5] = D3DXVECTOR3(  1.0f,  1.0f, 0.0f);
	vtx[6] = D3DXVECTOR3(  1.0f,  1.0f, 1.0f);
	vtx[7] = D3DXVECTOR3( -1.0f,  1.0f, 1.0f);

	D3DXMatrixInverse( &matViewProj, NULL, &matViewProj );

	// 뷰 행렬 * 프로젝션 행렬을 역행렬하면 월드좌표계의 프러스텀 좌표를 얻을 수 있다.
	for( int i=0; i<8; i++ )
	{
		D3DXVec3TransformCoord( &vtx[i], &vtx[i], &matViewProj );
	}

	// frustum을 구축한다. 노말 방향은 바깥쪽
	D3DXPlaneFromPoints( &m_plFrustum[0], &vtx[2], &vtx[6], &vtx[7] );// 원 평면(far)
	D3DXPlaneFromPoints( &m_plFrustum[1], &vtx[0], &vtx[3], &vtx[7] );// 좌 평면(left)
	D3DXPlaneFromPoints( &m_plFrustum[2], &vtx[1], &vtx[5], &vtx[6] );// 우 평면(right)
	D3DXPlaneFromPoints( &m_plFrustum[3], &vtx[4], &vtx[7], &vtx[6] );// 상 평면(top)
	D3DXPlaneFromPoints( &m_plFrustum[4], &vtx[0], &vtx[1], &vtx[2] );// 하 평면(bottom)
	D3DXPlaneFromPoints( &m_plFrustum[5], &vtx[0], &vtx[4], &vtx[5] );// 근 평면(near)

	/*	
	// Left plane
	m_plFrustum[FRUSTUM_PLANE_LEFT].a = matViewProj._14 + matViewProj._11;
	m_plFrustum[FRUSTUM_PLANE_LEFT].b = matViewProj._24 + matViewProj._21;
	m_plFrustum[FRUSTUM_PLANE_LEFT].c = matViewProj._34 + matViewProj._31;
	m_plFrustum[FRUSTUM_PLANE_LEFT].d = matViewProj._44 + matViewProj._41;

	// Right plane
	m_plFrustum[FRUSTUM_PLANE_RIGHT].a = matViewProj._14 - matViewProj._11;
	m_plFrustum[FRUSTUM_PLANE_RIGHT].b = matViewProj._24 - matViewProj._21;
	m_plFrustum[FRUSTUM_PLANE_RIGHT].c = matViewProj._34 - matViewProj._31;
	m_plFrustum[FRUSTUM_PLANE_RIGHT].d = matViewProj._44 - matViewProj._41;

	// Top plane
	m_plFrustum[FRUSTUM_PLANE_TOP].a = matViewProj._14 - matViewProj._12;
	m_plFrustum[FRUSTUM_PLANE_TOP].b = matViewProj._24 - matViewProj._22;
	m_plFrustum[FRUSTUM_PLANE_TOP].c = matViewProj._34 - matViewProj._32;
	m_plFrustum[FRUSTUM_PLANE_TOP].d = matViewProj._44 - matViewProj._42;

	// Bottom plane
	m_plFrustum[FRUSTUM_PLANE_BOTTOM].a = matViewProj._14 + matViewProj._12;
	m_plFrustum[FRUSTUM_PLANE_BOTTOM].b = matViewProj._24 + matViewProj._22;
	m_plFrustum[FRUSTUM_PLANE_BOTTOM].c = matViewProj._34 + matViewProj._32;
	m_plFrustum[FRUSTUM_PLANE_BOTTOM].d = matViewProj._44 + matViewProj._42;

	// Near plane
	m_plFrustum[FRUSTUM_PLANE_NEAR].a = matViewProj._14 + matViewProj._13;
	m_plFrustum[FRUSTUM_PLANE_NEAR].b = matViewProj._24 + matViewProj._23;
	m_plFrustum[FRUSTUM_PLANE_NEAR].c = matViewProj._34 + matViewProj._33;
	m_plFrustum[FRUSTUM_PLANE_NEAR].d = matViewProj._44 + matViewProj._43;

	// Far plane
	m_plFrustum[FRUSTUM_PLANE_FAR].a = matViewProj._14 - matViewProj._13;
	m_plFrustum[FRUSTUM_PLANE_FAR].b = matViewProj._24 - matViewProj._23;
	m_plFrustum[FRUSTUM_PLANE_FAR].c = matViewProj._34 - matViewProj._33;
	m_plFrustum[FRUSTUM_PLANE_FAR].d = matViewProj._44 - matViewProj._43;

	// Normalize planes
	for( int i = 0; i < 6; ++i )
	{
	D3DXPlaneNormalize( &m_plFrustum[i], &m_plFrustum[i] );
	}
	*/	
}

bool sunFrustum::VertexIsInFrustum( D3DXVECTOR3* pv )
{
	for( int i = 0; i < 6; ++i )
	{
		float fDist = D3DXPlaneDotCoord( &m_plFrustum[i], pv );
		if( fDist > 0.0f )
			return false;
	}

	return true;
}

bool sunFrustum::VertexIsInFrustum( int iCount, D3DXVECTOR3* pv )
{
	int i, j;

	for( j = 0; j < iCount; ++j )
	{
		if( !pv[j] )
			return false;
	}

	for( i = 0; i < 6; ++i )
	{
		for( j = 0; j < iCount; ++j )
		{
			float fDist = D3DXPlaneDotCoord( &m_plFrustum[i], &pv[j] );
			if( fDist > 0.0f )
				return false;
			else
				return true;
		}

	}

	return true;
}


bool sunFrustum::SphereIsInFrustum( D3DXVECTOR3* pv, float radius ) 
{
	for( int  i = 0 ; i < 6; ++i )
	{
		float fDist = D3DXPlaneDotCoord( &m_plFrustum[i], pv );
		if( fDist > radius )
			return false;
	}

	return true;
}