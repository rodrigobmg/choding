#include "GdsCamera.h"
#include "Renderer\GdsRendererDX9.h"

GdsCamera::GdsCamera() :
m_fDelta( 1.0f )
{
	SetName( OBJECT_NODE_CAMERA );
	vClear();
}

GdsCamera::~GdsCamera()
{
	vClear();
}

void GdsCamera::vClear()
{
}


void GdsCamera::vUpdate( float fElapsedtime )
{
	//카메라 뷰 변환 행렬
// 	D3DXVECTOR3 n = vLookatPt - GetWorldTanslate();
// 	n.Normalize();
// 	D3DXVECTOR3 v = vUpVec.Cross( n );
// 	v.Normalize();
// 	D3DXVECTOR3 u = n.Cross( v );
// 
// 	matView._11 = v[0];	matView._12 = u[0];	matView._13 = n[0];	matView._14 = 0;
// 	matView._21 = v[1];	matView._22 = u[1];	matView._23 = n[1];	matView._24 = 0;
// 	matView._31 = v[2];	matView._32 = u[2];	matView._33 = n[2];	matView._34 = 0;
//  	matView._41 = -( GetWorldTanslate().Dot( v ) );
//  	matView._42 = -( GetWorldTanslate().Dot( u ) );
//  	matView._43 = -( GetWorldTanslate().Dot( n ) );
// 	matView._44 = 1;
// 
// 	GetRotate().SetRow( 0 , matView._11 , matView._12 , matView._13 );
// 	GetRotate().SetRow( 1 , matView._21 , matView._22 , matView._23 );
// 	GetRotate().SetRow( 2 , matView._31 , matView._32 , matView._33 );

	
	/*
	zaxis = normal(At - Eye)
	xaxis = normal(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)

	xaxis.x           yaxis.x           zaxis.x          0
	xaxis.y           yaxis.y           zaxis.y          0
	xaxis.z           yaxis.z           zaxis.z          0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1*/


//  	D3DXVECTOR3 vEyePt   ( 0.0f, 3.0f,-5.0f );
//  	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
//  	D3DXVECTOR3 vUpVec   ( 0.0f, 1.0f, 0.0f );
 	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );


	if ( m_Frustum.m_bOrtho )
	{
		D3DXMatrixOrthoOffCenterLH( &matProj , m_Frustum.m_fLeft , m_Frustum.m_fRight , m_Frustum.m_fBottom , m_Frustum.m_fTop , m_Frustum.m_fZnear , m_Frustum.m_fFar );
	}	
	else
	{
		D3DXMatrixPerspectiveOffCenterLH( &matProj , m_Frustum.m_fLeft , m_Frustum.m_fRight , m_Frustum.m_fBottom , m_Frustum.m_fTop , m_Frustum.m_fZnear , m_Frustum.m_fFar );
	}

	m_Frustum.UpdatePlane( matView * matProj );
}

void GdsCamera::SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up )
{
	vEyePt = eye;
	vLookatPt = lookat;
	vUpVec	= up;
}