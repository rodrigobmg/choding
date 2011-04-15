#ifndef _GDS_SCENENODE_CAMERA_H_
#define _GDS_SCENENODE_CAMERA_H_

#include "../GdsProperty.h"
#include "../Transform\GdsFrustum.h"

class GdsFrustum;

class GdsCamera : public GdsProperty
{		

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
	GdsCamera();
	virtual ~GdsCamera();

	void			SetLootAtLH( GdsVector3& eye , GdsVector3& lookat , GdsVector3& up );
	void			SetLookAt( GdsVector3& lookat );
	void			SetEye( GdsVector3& eye );
	void			SetUp( GdsVector3& up );
	
	void			SetFrustum( GdsFrustum& Frustum ){ m_Frustum = Frustum; }
	GdsFrustum&		GetFrustum(){ return m_Frustum; }
};

typedef boost::shared_ptr< GdsCamera >	GdsCameraPtr;

#endif