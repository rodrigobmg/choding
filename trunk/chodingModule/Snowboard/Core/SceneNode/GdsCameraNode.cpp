#include "GdsCameraNode.h"

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

HRESULT GdsCameraNode::vRender()
{

}

HRESULT GdsCameraNode::vInitGeometry( float fElapsedtime )
{
	D3DXMatrixLookAtLH( &matView , &vEyePt , &vLookatPt , &vUpVec );
	GdsRendererDX9::GetDevice()->SetTransform( D3DTS_VIEW , &matView );
}

void GdsCameraNode::SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up )
{
	vEyePt = eye;
	vLookatPt = lookat;
	vUpVec	= up;
	D3DXMatrixLookAtLH( &matView , eye , lookat , up );
}
