#include "stdafx.h"

#include "EntityEvent.h"
#include "EntitySystem.h"
#include "Component\Visual\StaticMesh.h"
#include "Component\Visual\SkinnedMesh.h"
#include "Component\Visual\MorphingMesh.h"
#include "Component\IComponent.h"

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
	pEntity->SetEntityID( entitytype );
	m_entity_container.push_back( pEntity );
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

IComponent* EntitySystem::MakeComponent( int32 comtype )
{
	IComponent* pComponent = NULL;

	switch ( comtype )
	{
	case IComponent::STATIC_MESH:
		pComponent = new StaticMesh;
		break;

	case IComponent::SKINNED_MESH:
		pComponent = new SkinnedMesh;
		break;

	case IComponent::MORPHING_MESH:
		pComponent = new MorphingMesh;
		break;
	}

	return pComponent;
}