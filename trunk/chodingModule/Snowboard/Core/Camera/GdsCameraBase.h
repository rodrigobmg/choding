#ifndef _GDS_CAMERABASE_H_
#define _GDS_CAMERABASE_H_

#include "../Base/GdsObject.h"

class GdsCameraBase : public GdsObject
{

protected:

	virtual void	vClear() = 0;

public:

	GdsCameraBase(void){ SetName( OBJECT_CAMERA_BASE ); }
	virtual ~GdsCameraBase(void) {};

};


#endif
