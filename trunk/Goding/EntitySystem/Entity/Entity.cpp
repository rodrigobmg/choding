#include "stdafx.h"

#include "Entity.h"


Entity::Entity(void)
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

void Entity::SetComponent( int32 id , IComponent* pCom )
{
	if( pCom == NULL )
		return;

	pCom->SetEntity( this );

	m_Components.insert( std::pair<int32 , IComponent*>( id , pCom ) );
}