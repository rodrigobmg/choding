#include "GdsCameraNode.h"
#include "Renderer\GdsRendererDX9.h"

GdsCameraNode::GdsCameraNode():
m_bOrtho(false)
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

void GdsCameraNode::vRender( float fElapsedtime )
{

}

void GdsCameraNode::vInitGeometry( float fElapsedtime )
{
	//카메라 뷰 변환 행렬
	GdsVector3 n = vLookatPt - m_matWorld.m_Translate;
	n.Unitize();
	GdsVector3 v = vUpVec.Cross( n );
	v.Unitize();
	GdsVector3 u = n.Cross( v );
	
	D3DXMATRIXA16 view;
	D3DXMatrixIdentity( &view );
	view._11 = v[0];	view._12 = u[0];	view._13 = n[0];
	view._21 = v[1];	view._22 = u[1];	view._23 = n[1];
	view._31 = v[2];	view._32 = u[2];	view._33 = n[2];
	view._41 = -( m_matWorld.m_Translate.Dot( v ) );
	view._42 = -( m_matWorld.m_Translate.Dot( u ) );
	view._43 = -( m_matWorld.m_Translate.Dot( n ) );
	GetDevice()->SetTransform( D3DTS_VIEW , &view );

	D3DXMATRIXA16 proj;
	D3DXMatrixIdentity( &proj );

	if ( m_bOrtho )
	{
		proj._11 = 2*m_fnear / m_fWidth; 
		proj._22 = 2*m_fnear / m_fnear;
		proj._33 = m_ffar / ( m_ffar-m_fnear ); proj._34 = 1;
		proj._43 = -( m_ffar* m_fnear) / ( m_ffar - m_fnear ); proj._44 = 0;
	}
	else
	{
		proj._11 = 2 / m_fWidth;
		proj._22 = 2 / m_fnear;
		proj._33 = 1 / ( m_ffar - m_fnear ); proj._34 = 1;
		proj._43 = (-m_fnear) / ( m_ffar - m_fnear );
		//D3DXMatrixPerspectiveFovLH( &matProj, fov , aspect , znear , zfar );
		
	}	
	GetDevice()->SetTransform( D3DTS_PROJECTION, &proj );
}

void GdsCameraNode::SetLootAtLH( GdsVector3& eye , GdsVector3& lookat , GdsVector3& up )
{
	vEyePt = eye;
	vLookatPt = lookat;
	vUpVec	= up;
	m_matLocal.m_Translate.x = eye.x;
	m_matLocal.m_Translate.y = eye.y;
	m_matLocal.m_Translate.z = eye.z;
}

void GdsCameraNode::SetPerspective( float fov , float aspect , float znear , float zfar )
{
	this->fov = fov;
	this->aspect = aspect;
	this->znear = znear;
	this->zfar = zfar;
}

void GdsCameraNode::SetPerspective2( float width , float height , float znear , float zfar , bool bOrtho /*= false */ )
{
	m_ffar = zfar;
	m_fHeight = height;
	m_fWidth = width;
	m_fnear = znear;
	m_bOrtho = bOrtho;
}
