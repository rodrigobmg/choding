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

void GdsCameraNode::vRender()
{

}

void GdsCameraNode::vInitGeometry()
{
	GetDevice()->SetTransform( D3DTS_VIEW , &matView );
	GetDevice()->SetTransform( D3DTS_PROJECTION, &matProj );
}

void GdsCameraNode::SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up )
{
	vEyePt = eye;
	vLookatPt = lookat;
	vUpVec	= up;
	D3DXMatrixLookAtLH( &matView , &vEyePt , &vLookatPt , &vUpVec );
}

void GdsCameraNode::SetPerspective( float fov , float aspect , float znear , float zfar )
{
	this->fov = fov;
	this->aspect = aspect;
	this->znear = znear;
	this->zfar = zfar;
	
	D3DXMatrixPerspectiveFovLH( &matProj, fov , aspect , znear , zfar );
}

