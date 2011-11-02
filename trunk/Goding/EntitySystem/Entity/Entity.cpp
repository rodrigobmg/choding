#include "stdafx.h"

#include "Entity.h"


Entity::Entity(void):
m_bActivated(true),
m_pLinkedTo(NULL)
{
}


Entity::~Entity(void)
{
}

void Entity::DispatchEvent( EntityEvent& event )
{
	switch( event.itype )
	{
	case EntityEvent::TRANS_XFORM:


		break;

	case EntityEvent::RENDER:
		break;
	}
}

void Entity::SetComponent( COMPONENT_TYPE id , IComponent* pCom )
{
	if( pCom == NULL )
		return;

	pCom->SetEntity( this );

	m_Components.insert( std::pair<COMPONENT_TYPE , IComponent*>( id , pCom ) );
}

IComponent* Entity::GetComponent( COMPONENT_TYPE id )
{
 	typedef std::map< COMPONENT_TYPE , IComponent* >component_table_t;
 	component_table_t::iterator it = m_Components.find( id );
 	if ( it != m_Components.end() )
 		return it->second;
 	else
		return NULL;
}
