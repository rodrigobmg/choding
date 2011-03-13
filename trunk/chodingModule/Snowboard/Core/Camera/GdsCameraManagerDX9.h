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

public:

	GdsCameraManagerDX9(void);
	virtual ~GdsCameraManagerDX9(void);

	virtual	HRESULT		Create();

	GdsCameraNodePtr	GetCamNode( int iCamIndex );

	void				Attach( GdsCameraNodePtr camnode );
	void				Detach( int32_t index );
	void				DetachAll();

};

typedef boost::shared_ptr< GdsCameraManagerDX9 > GdsCameraManagerDX9Ptr;

#endif
