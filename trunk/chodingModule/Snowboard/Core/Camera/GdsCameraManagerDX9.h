#ifndef _GDS_CAMERAMANAGERDX9_H_
#define _GDS_CAMERAMANAGERDX9_H_

#include "GdsCameraBase.h"
#include "../SceneNode/GdsCameraNode.h"

class GdsCameraManagerDX9 : public GdsCameraBase 
{

private:

	LPDIRECT3DDEVICE9		m_pDevice;
	typedef std::vector< GdsCameraNodePtr >		CAM_CONTAINER;	
	CAM_CONTAINER	m_camaraContainer;

	int32_t			m_iCurCamIndex;

protected:

	virtual void	vClear();
	

public:

	GdsCameraManagerDX9(void);
	virtual ~GdsCameraManagerDX9(void);

	virtual	HRESULT		Create( LPDIRECT3DDEVICE9 device );
	GdsCameraNodePtr	GetCamNode( int iCamIndex );

	void				Attach( GdsCameraNodePtr camnode );
	void				Detach( int32_t index );
	void				DetachAll();

	void				SetCurCam( int32_t index ){ m_iCurCamIndex = index; }
	int32_t				GetCurCam(){ return m_iCurCamIndex; }

	void				Update( float fElapsedTime );
};

typedef boost::shared_ptr< GdsCameraManagerDX9 > GdsCameraManagerDX9Ptr;

#endif
