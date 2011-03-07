#ifndef _GDS_CAMERAMANAGERDX9_H_
#define _GDS_CAMERAMANAGERDX9_H_

#include "GdsCameraBase.h"
#include "../SceneNode/GdsCameraNode.h"

class GdsCameraManagerDX9 : public GdsCameraBase
{

protected:

	virtual void	vClear();

	typedef std::vector< GdsCameraNodePtr >		CAM_CONTAINER;	
	CAM_CONTAINER	m_camaraContainer;

	int				m_iCurCamIndex;
public:

	GdsCameraManagerDX9(void);
	virtual ~GdsCameraManagerDX9(void);

	virtual		void	SetCam( int index );
	virtual		void	Attach( GdsCameraNodePtr camnode );
	virtual		void	Detach( int index );
	virtual		void	DetachAll();
	void				Update( float fElapsedtime );
};

typedef boost::shared_ptr< GdsCameraManagerDX9 > GdsCameraManagerDX9Ptr;

#endif
