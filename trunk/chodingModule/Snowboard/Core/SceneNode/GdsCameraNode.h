#ifndef _SNOWBOARD_SCENENODE_CAMERANODE_
#define _SNOWBOARD_SCENENODE_CAMERANODE_

#include "GdsNode.h"

class GdsCameraNode : public GdsNode{		

protected:
	
	virtual	void	vInitGeometry( float fElapsedtime );
	virtual void	vRender( float fElapsedtime );
	virtual	void	vClear();

	GdsVector3		vEyePt;
	GdsVector3		vLookatPt;
	GdsVector3		vUpVec;
	GdsMatrix3	matView;
	
	float			fov;
	float			aspect;
	float			znear;
	float			zfar;
	D3DXMATRIXA16	matProj;

	float			m_fWidth;
	float			m_fHeight;
	float			m_fnear;
	float			m_ffar;

	bool			m_bOrtho;
public:
	GdsCameraNode();
	virtual ~GdsCameraNode();

	void			SetLootAtLH( GdsVector3& eye , GdsVector3& lookat , GdsVector3& up );
	GdsMatrix3&		GetViewMatrix() { return matView; }
	
	void			SetLookAt( GdsVector3& lookat );
	void			SetEye( GdsVector3& eye );
	void			SetUp( GdsVector3& up );



	void			SetPerspective( float fov , float aspect , float znear , float zfar );
	void			SetPerspective2( float width , float height , float znear , float zfar , bool bOrtho = false );
	//GdsMatrix3&		GetProjMatrix() { return matProj; }
	
};

typedef boost::shared_ptr< GdsCameraNode >	GdsCameraNodePtr;

#endif