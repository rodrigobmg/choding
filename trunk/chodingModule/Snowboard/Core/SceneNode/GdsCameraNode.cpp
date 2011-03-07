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
	D3DXMatrixLookAtLH( &matView , &vEyePt , &vLookatPt , &vUpVec );
}

void GdsCameraNode::SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up )
{
	vEyePt = eye;
	vLookatPt = lookat;
	vUpVec	= up;
	D3DXMatrixLookAtLH( &matView , &vEyePt , &vLookatPt , &vUpVec );
}
