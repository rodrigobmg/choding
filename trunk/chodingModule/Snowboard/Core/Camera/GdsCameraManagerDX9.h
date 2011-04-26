#ifndef _GDS_CAMERAMANAGERDX9_H_
#define _GDS_CAMERAMANAGERDX9_H_

#include "GdsCamera.h"
#include "../Base/GdsObject.h"

class GdsCameraManagerDX9 : public GdsObject
{

private:

	typedef std::vector< GdsCameraPtr >		CAM_CONTAINER;	
	CAM_CONTAINER			m_camaraContainer;
	int32_t					m_iCurCamIndex;

protected:

	virtual void	vClear();
	

public:

	GdsCameraManagerDX9(void);
	virtual ~GdsCameraManagerDX9(void);

	GdsCameraPtr		GetCamNode( int iCamIndex );

	void				Attach( GdsCameraPtr camnode );
	void				Detach( int32_t index );
	void				DetachAll();

	void				SetCurCam( int32_t index ){ m_iCurCamIndex = index; }
	GdsCameraPtr		GetCurCam(){ return m_camaraContainer.at( m_iCurCamIndex); }

};

typedef boost::shared_ptr< GdsCameraManagerDX9 > GdsCameraManagerDX9Ptr;

#define CAMMGR	singleton_holder< GdsCameraManagerDX9 >::instance()

#endif
