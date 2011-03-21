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
}

void GdsBillboardNode::vRender( float fElapsedtime )
{

}

void GdsBillboardNode::vClear()
{

}
