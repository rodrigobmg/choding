#ifndef _SNOWBOARD_SCENENODE_CAMERANODE_
#define _SNOWBOARD_SCENENODE_CAMERANODE_

#include "GdsNode.h"

class GdsCameraNode : public GdsNode{		

protected:
	
	virtual void	vInitGeometry();	
	virtual void	vRender();
	virtual	void	vClear();

	D3DXVECTOR3		vEyePt;
	D3DXVECTOR3		vLookatPt;
	D3DXVECTOR3		vUpVec;
	D3DXMATRIXA16	matView;

public:
	GdsCameraNode();
	virtual ~GdsCameraNode();

	void			SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up );
	D3DXMATRIXA16&	GetViewMatrix() { return matView; }

	
};

typedef boost::shared_ptr< GdsCameraNode >	GdsCameraNodePtr;

#endif