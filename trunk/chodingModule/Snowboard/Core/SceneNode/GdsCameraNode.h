#ifndef _SNOWBOARD_SCENENODE_CAMERANODE_
#define _SNOWBOARD_SCENENODE_CAMERANODE_

#include "GdsNode.h"

class GdsCameraNode : public GdsNode{		

protected:
	
	virtual	void	vInitGeometry( float fElapsedtime );
	virtual void	vRender( float fElapsedtime );
	virtual	void	vClear();

	D3DXVECTOR3		vEyePt;
	D3DXVECTOR3		vLookatPt;
	D3DXVECTOR3		vUpVec;
	D3DXMATRIXA16	matView;
	
	float			fov;
	float			aspect;
	float			znear;
	float			zfar;
	D3DXMATRIXA16	matProj;

public:
	GdsCameraNode();
	virtual ~GdsCameraNode();

	void			SetLootAtLH( D3DXVECTOR3& eye , D3DXVECTOR3& lookat , D3DXVECTOR3& up );
	D3DXMATRIXA16&	GetViewMatrix() { return matView; }

	void			SetPerspective( float fov , float aspect , float znear , float zfar );
	D3DXMATRIXA16&	GetProjMatrix() { return matProj; }
	
};

typedef boost::shared_ptr< GdsCameraNode >	GdsCameraNodePtr;

#endif