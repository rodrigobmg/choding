#include "stdafx.h"
#include "EntitySystem.h"
#include "EntityEvent.h"

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
	typedef std::vector< IEntity* >::iterator iter;
	iter begin = m_entity_container.begin();
	iter end = m_entity_container.end();
	for ( iter it = begin ; it != end ; ++it )
	{
		*(it)->DispatchEvent( TRANS_XFORM );
	}
}
