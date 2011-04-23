#include "GdsCameraNode.h"
#include "Renderer\GdsRendererDX9.h"

GdsCameraNode::GdsCameraNode()
{
	SetName( OBJECT_NODE_CAMERA );
	vClear();
}

GdsCameraNode::~GdsCameraNode()
{
	vClear();
}

void GdsCameraNode::vClear()
{
}


void GdsCameraNode::vInitGeometry( float fElapsedtime )
{
	//카메라 뷰 변환 행렬

	
	/*
	zaxis = normal(At - Eye)
	xaxis = normal(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)

	xaxis.x           yaxis.x           zaxis.x          0
	xaxis.y           yaxis.y           zaxis.y          0
	xaxis.z           yaxis.z           zaxis.z          0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1*/


// 	D3DXVECTOR3 vEyePt   ( 0.0f, 3.0f,-5.0f );
// 	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
// 	D3DXVECTOR3 vUpVec   ( 0.0f, 1.0f, 0.0f );
// 	D3DXMATRIXA16 matView;
// 	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );

//	GetDevice()->SetTransform( D3DTS_VIEW , &matView );

	if ( m_Frustum.m_bOrtho )
	{
		D3DXMatrixOrthoOffCenterLH( &matProj , m_Frustum.m_fLeft , m_Frustum.m_fRight , m_Frustum.m_fBottom , m_Frustum.m_fTop , m_Frustum.m_fZnear , m_Frustum.m_fFar );
	}	
	else
	{
		D3DXMatrixPerspectiveOffCenterLH( &matProj , m_Frustum.m_fLeft , m_Frustum.m_fRight , m_Frustum.m_fBottom , m_Frustum.m_fTop , m_Frustum.m_fZnear , m_Frustum.m_fFar );
	}

//	GetDevice()->SetTransform( D3DTS_PROJECTION, &matProj );
}

void GdsCameraNode::SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up )
{
	vEyePt = eye;
	vLookatPt = lookat;
	vUpVec	= up;
// 	GetTranslate().x = eye.x;
// 	GetTranslate().y = eye.y;
// 	GetTranslate().z = eye.z;
}