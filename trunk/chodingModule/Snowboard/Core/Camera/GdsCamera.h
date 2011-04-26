#ifndef _SNOWBOARD_SCENENODE_CAMERA_H_
#define _SNOWBOARD_SCENENODE_CAMERA_H_

#include "GdsFrustum.h"

class GdsFrustum;

class GdsCamera : public GdsObject
{	
private:

protected:
		
 	D3DXVECTOR3		vEyePt;
 	D3DXVECTOR3		vLookatPt;
 	D3DXVECTOR3		vUpVec;
	
	D3DXMATRIX		matView;
	D3DXMATRIX		matProj;

	GdsFrustum		m_Frustum;

	bool			m_bVisibleFrustum;
	

	virtual	void	vUpdate( float fElapsedtime );
	virtual	void	vClear();

public:
	GdsCamera();
	virtual ~GdsCamera();

	void			SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up );
	void			SetLookAt( D3DXVECTOR3& lookat );
	void			SetEye( D3DXVECTOR3& eye );
	void			SetUp( D3DXVECTOR3& up );
	
	void			SetFrustum( GdsFrustum& Frustum ){ m_Frustum = Frustum; }
	GdsFrustum&		GetFrustum(){ return m_Frustum; }
	void			Update( float fElapsedtime){ vUpdate( fElapsedtime ); }

	D3DXMATRIX&		GetViewMat(){ return matView; }
	D3DXMATRIX&		GetProjMat(){ return matProj; }

	void			SetVisibleFrustum( bool bflag );
};

typedef boost::shared_ptr< GdsCamera >	GdsCameraPtr;

#endif