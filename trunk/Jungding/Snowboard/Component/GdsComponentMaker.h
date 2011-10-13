#ifndef _SNOWBOARD_COMPONENTMAKER_H_
#define _SNOWBOARD_COMPONENTMAKER_H_

class GdsComponentMaker
{
public:
	GdsComponentMaker();
	virtual ~GdsComponentMaker();

	GdsComponent*		MakeComponent( GdsComponent::goc_id_type id );

private:

};


#endif