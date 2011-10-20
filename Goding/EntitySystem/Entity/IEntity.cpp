#include "stdafx.h"

#include "IEntity.h"


IEntity::IEntity(void)
{
}


IEntity::~IEntity(void)
{
}

void IEntity::DispatchEvent( EntityEvent event )
{
	switch( event.itype )
	{
	case EntityEvent::TRANS_XFORM:
		break;

	case EntityEvent::RENDER:
		break;
	}
}