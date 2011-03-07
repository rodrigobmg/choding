#ifndef _GDS_CAMERAMANAGERDX9_H_
#define _GDS_CAMERAMANAGERDX9_H_

#include "GdsCameraBase.h"
#include "../SceneNode/GdsCameraNode.h"

class GdsCameraManagerDX9 : public GdsCameraBase
{

protected:

	LPDIRECT3DDEVICE9	m_pDevice;

	virtual void	vClear();

	typedef std::vector< GdsCameraNodePtr >		CAM_CONTAINER;	
	CAM_CONTAINER	m_camaraContainer;

	int32_t				m_iCurCamIndex;
public:

	GdsCameraManagerDX9(void);
	virtual ~GdsCameraManagerDX9(void);

	virtual		void	SetCam( int32_t index );
	virtual		void	Attach( GdsCameraNodePtr camnode );
	virtual		void	Detach( int32_t index );
	virtual		void	DetachAll();
	void				Update( float fElapsedtime );
};

typedef boost::shared_ptr< GdsCameraManagerDX9 > GdsCameraManagerDX9Ptr;

#endif
