#include "GdsBillboardNode.h"
#include "Camera\GdsCameraManagerDX9.h"
#include "GdsCameraNode.h"

GdsBillboardNode::GdsBillboardNode()
{

}

GdsBillboardNode::~GdsBillboardNode()
{

}

void GdsBillboardNode::vInitGeometry( float fElapsedtime )
{
	int32_t cur_cam_index = CAMMGR.GetCurCam();
	GdsCameraNodePtr pCamera = CAMMGR.GetCamNode( cur_cam_index );
	GdsMatrix3 billboard;
	billboard.MakeIdentity();
	GdsMatrix3 camMat = pCamera->GetWorldTransform().m_Rotate;
	billboard.SetEntry( 0 , 0 , camMat.m_pEntry[0][0] );
	billboard.SetEntry( 0 , 2 , camMat.m_pEntry[0][2] );
	billboard.SetEntry( 2 , 0 , camMat.m_pEntry[2][0] );
	billboard.SetEntry( 2 , 2 , camMat.m_pEntry[2][2] );
	billboard = billboard.Inverse();

	pCamera->GetWorldTransform().m_Rotate = billboard;
	
}

void GdsBillboardNode::vRender( float fElapsedtime )
{

}

void GdsBillboardNode::vClear()
{

}
