#ifndef _SNOWBOARD_SCENENODE_CAMERANODE_
#define _SNOWBOARD_SCENENODE_CAMERANODE_

#include "GdsNode.h"
#include "../Transform\GdsFrustum.h"

class GdsFrustum;

class GdsCameraNode : public GdsNode{		

protected:
	
	D3DXMATRIXA16	matProj;
	D3DXMATRIXA16	matView;

	D3DXVECTOR3		vEyePt;
	D3DXVECTOR3		vLookatPt;
	D3DXVECTOR3		vUpVec;
	
	GdsFrustum		m_Frustum;

	virtual	void	vInitGeometry( float fElapsedtime );
	virtual	void	vClear();

public:
	GdsCameraNode();
	virtual ~GdsCameraNode();

	void			SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up );
	void			SetLookAt( D3DXVECTOR3& lookat );
	void			SetEye( D3DXVECTOR3& eye );
	void			SetUp( D3DXVECTOR3& up );
	
	void			SetFrustum( GdsFrustum& Frustum ){ m_Frustum = Frustum; }
	GdsFrustum&		GetFrustum(){ return m_Frustum; }
};

typedef boost::shared_ptr< GdsCameraNode >	GdsCameraNodePtr;

#endif