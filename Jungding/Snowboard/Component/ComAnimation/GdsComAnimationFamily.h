#ifndef _SNOWBOARD_COMPONENT_ANIMAION_FAMILY_H_
#define _SNOWBOARD_COMPONENT_ANIMAION_FAMILY_H_

class GdsComAnimationFamily : public GdsComponent
{
public:

	GdsComAnimationFamily();
	virtual ~GdsComAnimationFamily();

	virtual const goc_id_type familyID() const	{ return COMPONENT_VISUAL_FAMILY; }

};

#endif