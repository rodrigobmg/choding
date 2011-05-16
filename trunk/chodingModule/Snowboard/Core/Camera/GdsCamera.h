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

	float			m_fDelta; // 카메라 연산에 쓰일 기본 델타값 기본값 1.0f

	virtual	void	vUpdate( float fElapsedtime );
	virtual	void	vClear();

public:
	GdsCamera();
	virtual ~GdsCamera();
	
	void			SetDelta( float fvalue ){ m_fDelta = fvalue; }

	void			SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up );

	void			SetLookAt( D3DXVECTOR3& lookat ){ vLookatPt = lookat; }
	D3DXVECTOR3&	GetLookAt(){ return vLookatPt; }

	void			SetEye( D3DXVECTOR3& eye ){ vEyePt = eye; }
	D3DXVECTOR3&	GetEye(){ return vEyePt; }

	void			SetUp( D3DXVECTOR3& up ){ vUpVec = up; }
	D3DXVECTOR3&	GetUp(){ return vUpVec; }
	
	void			SetFrustum( GdsFrustum& Frustum ){ m_Frustum = Frustum; }
	GdsFrustum&		GetFrustum(){ return m_Frustum; }
	void			Update( float fElapsedtime){ vUpdate( fElapsedtime ); }

	D3DXMATRIX&		GetViewMat(){ return matView; }
	D3DXMATRIX&		GetProjMat(){ return matProj; }

};

typedef boost::shared_ptr< GdsCamera >	GdsCameraPtr;

#endif