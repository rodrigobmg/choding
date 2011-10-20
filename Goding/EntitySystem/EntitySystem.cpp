#include "stdafx.h"

#include "EntityEvent.h"
#include "EntitySystem.h"

EntitySystem::EntitySystem()
{

}

EntitySystem::~EntitySystem()
{

}

IEntity* EntitySystem::GetEntity( int32 entityid )
{
	typedef std::vector< IEntity* >::iterator iter;
	iter begin = m_entity_container.begin();
	iter end = m_entity_container.end();
	for ( iter it = begin ; it != end ; ++it )
	{
		if ( entityid == (*it)->GetEntityID() )
		{
			return *it;
		}
	}

	return NULL;
}

IEntity* EntitySystem::AllocEntity( int32 entitytype )
{
	IEntity* pEntity = new IEntity;
	return pEntity;
}

bool EntitySystem::ReleaseEntity( int32 entitytype )
{
	return true;
}

void EntitySystem::Update( float fAccumtime )
{
	EntityEvent event;
	event.itype = EntityEvent::TRANS_XFORM;
	event.fAccumtime = fAccumtime;

	typedef std::vector< IEntity* >::iterator iter;
	iter begin = m_entity_container.begin();
	iter end = m_entity_container.end();
	for ( iter it = begin ; it != end ; ++it )
	{
		(*it)->DispatchEvent( event );
	}
}
