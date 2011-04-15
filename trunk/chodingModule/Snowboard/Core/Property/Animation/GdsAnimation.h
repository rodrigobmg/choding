#ifndef _GDS_CORE_PROPERTY_ANIMATION_H_
#define _GDS_CORE_PROPERTY_ANIMATION_H_

#include "../GdsProperty.h"

class GdsAnimation : public GdsProperty
{
public:
	GdsAnimation(){};
	virtual ~GdsAnimation(){};
};

typedef boost::shared_ptr< GdsAnimation > GdsAnimationPtr;

#endif