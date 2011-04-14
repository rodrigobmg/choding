#ifndef _SNOWBOARD_SCENENODE_CAMERANODE_
#define _SNOWBOARD_SCENENODE_CAMERANODE_

#include "GdsNode.h"
#include "../Transform\GdsFrustum.h"

class GdsFrustum;

class GdsCameraNode : public GdsNode{		

protected:
	
	virtual	void	vInitGeometry( float fElapsedtime );
	virtual	void	vClear();

	GdsVector3		vEyePt;
	GdsVector3		vLookatPt;
	GdsVector3		vUpVec;
	D3DXMATRIXA16	matView;
	
	GdsFrustum		m_Frustum;
	D3DXMATRIXA16	matProj;

public:
	GdsCameraNode();
	virtual ~GdsCameraNode();

	void			SetLootAtLH( GdsVector3& eye , GdsVector3& lookat , GdsVector3& up );
	void			SetLookAt( GdsVector3& lookat );
	void			SetEye( GdsVector3& eye );
	void			SetUp( GdsVector3& up );
	
	void			SetFrustum( GdsFrustum& Frustum ){ m_Frustum = Frustum; }
	GdsFrustum&		GetFrustum(){ return m_Frustum; }
};

typedef boost::shared_ptr< GdsCameraNode >	GdsCameraNodePtr;

#endif