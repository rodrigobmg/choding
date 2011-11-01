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

Entity* EntitySystem::GetEntity( int32 entityid )
{
	typedef std::vector< Entity* >::iterator iter;
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

Entity* EntitySystem::AllocEntity()
{
	Entity* pEntity = new Entity;	
	m_entity_container.push_back( pEntity );
	pEntity->SetEntityID( m_entity_container.size() );
	return pEntity;
}

bool EntitySystem::ReleaseEntity( int32 entityid )
{
	return false;
}

void EntitySystem::Update( float fAccumtime )
{
	m_activated_entity_container.clear();

	EntityEvent event;
	event.itype = EntityEvent::TRANS_XFORM;
	event.fAccumtime = fAccumtime;

	typedef std::vector< Entity* >::iterator iter;
	iter begin = m_entity_container.begin();
	iter end = m_entity_container.end();
	for ( iter it = begin ; it != end ; ++it )
	{
		if ( (*it)->IsActivated() )
		{
			(*it)->DispatchEvent( event );
			m_activated_entity_container.push_back( (*it) );
		}		
	}
}

IComponent* EntitySystem::MakeComponent( IComponent::ENABLE_COMPONENT_TYPE comtype )
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

	//default:
		//ASSERT( pComponent );
	}

	if ( pComponent != NULL )
		m_component_container.push_back( pComponent );

	return pComponent;
}

void EntitySystem::Release()
{
	for_each( m_component_container.begin() , m_component_container.end() , functor::deleter() );
	for_each( m_entity_container.begin() , m_entity_container.end() , functor::deleter() );
}
