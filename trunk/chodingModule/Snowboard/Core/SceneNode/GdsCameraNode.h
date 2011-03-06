#ifndef _SNOWBOARD_SCENENODE_CAMERANODE_
#define _SNOWBOARD_SCENENODE_CAMERANODE_

#include "GdsMeshNode.h"

class GdsCameraNode : public GdsMeshNode{		

protected:
	
	virtual HRESULT vInitGeometry( float fElapsedtime );	
	virtual HRESULT vRender();
	virtual	void	vClear();

	D3DXVECTOR3		vEyePt;
	D3DXVECTOR3		vLookatPt;
	D3DXVECTOR3		vUpVec;
	D3DXMATRIXA16	matView;

public:
	GdsCameraNode();
	virtual ~GdsCameraNode();

	void			SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up );

	
};

typedef boost::shared_ptr< GdsCameraNode >	GdsCameraNodePtr;

#endif