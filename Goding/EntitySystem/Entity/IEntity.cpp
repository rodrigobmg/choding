#include "stdafx.h"

#include "IEntity.h"


IEntity::IEntity(void)
{
}


IEntity::~IEntity(void)
{
}

void IEntity::DispatchEvent( EntityEvent& event )
{
	switch( event.itype )
	{
	case EntityEvent::TRANS_XFORM:


		break;

	case EntityEvent::RENDER:
		break;
	}
}

void IEntity::SetComponent( int32 id , IComponent* pCom )
{
	m_Components.insert( std::pair<int32 , IComponent*>( id , pCom ) );
}