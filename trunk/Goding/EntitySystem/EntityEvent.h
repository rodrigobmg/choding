#ifndef _ENTITYEVENT_H_
#define _ENTITYEVENT_H_
struct EntityEvent
{
	enum EVENT_TYPE
	{
		NONE,
		TRANS_XFORM,
		RENDER,
	};

	EntityEvent()
	{
		itype = NONE;
		fAccumtime = 0.f;
	}

	EVENT_TYPE	itype;
	float	fAccumtime;	
};

#endif